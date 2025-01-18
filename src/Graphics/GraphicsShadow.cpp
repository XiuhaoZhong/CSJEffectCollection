#include "GraphicsShadow.h"

#include "Utils/utils.h"

GraphicsShadow::GraphicsShadow() {
    initShapes();
    setupLightsAndMaterial();

    m_torusPos = glm::vec3(1.6f, 0.0f, -0.3f);
    m_pyramidPos = glm::vec3(-1.0f, 0.1f, 0.3f);
    m_cameraPos = glm::vec3(0.0f, 0.2f, 6.0f);
    m_lightPos = glm::vec3(-3.8f, 2.2f, 1.1f);

    m_originPos = glm::vec3(0.0f, 0.0f, 0.0f);
    m_upDirection = glm::vec3(0.0f, 1.0f, 0.0f);
}

GraphicsShadow::~GraphicsShadow() {

}

bool GraphicsShadow::init(GLFWwindow *window) {
    string shadowVerPath = Utils::getResourcePath("shaders/shadowShaders/shadowVertexShader.glsl");
    string shadowFragPath = Utils::getResourcePath("shaders/shadowShaders/shadowFragmentShader.glsl");
    m_pShadowProgram = std::make_shared<GraphicProgram>(shadowVerPath, shadowFragPath);

    string renderVerPath = Utils::getResourcePath("shaders/shadowShaders/renderVertexShader.glsl");
    string renderFragPath = Utils::getResourcePath("shaders/shadowShaders/renderFragmentShader.glsl");
    m_pRenderProgram = std::make_shared<GraphicProgram>(renderVerPath, renderFragPath);

    //m_pSelectProgram = std::make_shared<GraphicProgram>(renderVerPath, renderFragPath);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float aspect = (float)width / (float)height;
    m_pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);

    m_iWinWidth = width;
    m_iWinHeight = height;

    setupShadowTex(width, height);

    m_toShadowMat = glm::mat4(0.5f, 0.0f, 0.0f, 0.0f,
                              0.0f, 0.5f, 0.0f, 0.0f,
                              0.0f, 0.0f, 0.5f, 0.0f,
                              0.5f, 0.5f, 0.5f, 1.0f);

    // Pyramid vertexes and normals.
    GraphicVec3Array pyramidVertices = m_pPyramid->getVertices();
    GraphicVec3Array pyramidNormals = m_pPyramid->getNormals();
    int pyramidVerNum = m_pPyramid->getVerticeNum();

    std::vector<float> pyramidPvalues;
    std::vector<float> pyramidNvalues;
    for (int i = 0; i < pyramidVerNum; i++) {
        pyramidPvalues.push_back(pyramidVertices[i].x);
        pyramidPvalues.push_back(pyramidVertices[i].y);
        pyramidPvalues.push_back(pyramidVertices[i].z);

        pyramidNvalues.push_back(pyramidNormals[i].x);
        pyramidNvalues.push_back(pyramidNormals[i].y);
        pyramidNvalues.push_back(pyramidNormals[i].z);
    }

    // Torus vertexes and normals.
    GraphicVec3Array torusVertices = m_pTorus->getVertices();
    GraphicVec3Array torusNormals = m_pTorus->getNormals();
    GraphicIntArray torusInd = m_pTorus->getIndices();
    int torusVerNum = m_pTorus->getVerticeNum();
    
    std::vector<float> torusPvalues;
    std::vector<float> torusNvalues;
    for (int i = 0; i < torusVerNum; i++) {
        torusPvalues.push_back(torusVertices[i].x);
        torusPvalues.push_back(torusVertices[i].y);
        torusPvalues.push_back(torusVertices[i].z);

        torusNvalues.push_back(torusNormals[i].x);
        torusNvalues.push_back(torusNormals[i].y);
        torusNvalues.push_back(torusNormals[i].z);
    }
    
    glGenVertexArrays(ShadowNumVAO, m_VAO);
    glBindVertexArray(m_VAO[0]);
    glGenBuffers(ShadowNumVBO, m_VBO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, pyramidPvalues.size() * 4, &pyramidPvalues[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, torusPvalues.size() * 4, &torusPvalues[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, pyramidNvalues.size() * 4, &pyramidNvalues[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, torusNvalues.size() * 4, &torusNvalues[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO[4]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, torusInd.size() * 4, &torusInd[0], GL_STATIC_DRAW);

    setWindow(window);

    return true;
}

void GraphicsShadow::display(GLFWwindow *window, double currentTime) {
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    m_curLightPos = glm::vec3(m_lightPos);
    m_lightVmatrix = glm::lookAt(m_curLightPos, m_originPos, m_upDirection);
    m_lightPmatrix = glm::perspective(Utils::toRadians(60.0f), 1.0f, 0.1f, 1000.0f);

    glBindFramebuffer(GL_FRAMEBUFFER, m_shadowBuffer);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_shadowTex, 0);

    glDrawBuffer(GL_NONE);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_POLYGON_OFFSET_FILL);
    //glPolygonOffset(2.0f, 4.0f);

    drawTexAtLight();

    //glDisable(GL_POLYGON_OFFSET_FILL);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_shadowTex);

    glDrawBuffer(GL_FRONT);

    drawContentAtCamera();
}

void GraphicsShadow::onMouseLButtonPressed() {
    selectedItem();
}

void GraphicsShadow::onMouseMove(double xCoord, double yCoord) {
    if (!MouseLButtonPressed()) {
        return;
    }

    glm::vec3 deltaPos = getMouseDeltaPos();
    
    /*deltaPos.x = -deltaPos.x;
    modifyCameraPos(deltaPos);*/

    double rotateSetp = 1.0f;
    if (deltaPos.x > 0) {
        m_dAngleDeltaX = m_dAngleDeltaX + rotateSetp > 60.0 ? 60.0 : m_dAngleDeltaX + rotateSetp;
    } else if (deltaPos.x < 0) {
        m_dAngleDeltaX = m_dAngleDeltaX - rotateSetp < -60.0 ? -60.0 : m_dAngleDeltaX - rotateSetp;
    }

    if (deltaPos.y > 0) {
        m_dAngleDeltaY = m_dAngleDeltaY + rotateSetp > 60.0 ? 60.0 : m_dAngleDeltaY + rotateSetp;
    } else if (deltaPos.y < 0) {
        m_dAngleDeltaY = m_dAngleDeltaY - rotateSetp < -60.0 ? -60.0 : m_dAngleDeltaY - rotateSetp;
    }
}

void GraphicsShadow::onMouseScroll(double xOffset, double yOffset) {
    if (yOffset > 0) {
        m_cameraPos.z -= 0.5;
    } else {
        m_cameraPos.z += 0.5;
    }
}

void GraphicsShadow::onMouseLButtonReleased() {
    
}

void GraphicsShadow::initShapes() {
    string objFilePath = Utils::getResourcePath("resources/pyr.obj");
    m_pPyramid = GraphicShapeBase::createPraymid(objFilePath);
    m_pTorus = GraphicShapeBase::createTorus(0.6f, 0.4f, 48);
}

void GraphicsShadow::setupLightsAndMaterial() {
    m_pGlobalAmt = Utils::globalAmbient();

    m_pLightAmt = Utils::lightAmbient();
    m_pLightDif = Utils::lightDiffuse();
    m_pLightSpe = Utils::lightSpecular();

    m_pBronzeAmt = Utils::bronzeAmbient();
    m_pBronzeDif = Utils::bronzeDiffuse();
    m_pBronzeSpe = Utils::bronzeSpecular();
    m_BronzeShininess = Utils::bronzeShininess();
}

void GraphicsShadow::setupShadowTex(int width, int height) {

    glGenFramebuffers(1, &m_shadowBuffer);
    glGenTextures(1, &m_shadowTex);

    glBindTexture(GL_TEXTURE_2D, m_shadowTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void GraphicsShadow::installLight(GraphicSpProgram &program, glm::mat4 vMat) {
    m_transformed = glm::vec3(vMat * glm::vec4(m_curLightPos, 1.0f));

    m_pLightPos[0] = m_transformed.x;
    m_pLightPos[1] = m_transformed.y;
    m_pLightPos[2] = m_transformed.z;

    GLuint globalAmbLoc = program->getUniformLoc("globalAmbient");
    GLuint lightAmbLoc = program->getUniformLoc("light.ambient");
    GLuint lightDiffLoc = program->getUniformLoc("light.diffuse");
    GLuint lightSpecLoc = program->getUniformLoc("light.specular");
    GLuint lightPosLoc = program->getUniformLoc("light.position");
    GLuint materialAmtLoc = program->getUniformLoc("material.ambient");
    GLuint materialDiffLoc = program->getUniformLoc("material.diffuse");
    GLuint materialSpecLoc = program->getUniformLoc("material.specular");
    GLuint materialShinLoc = program->getUniformLoc("material.shininess");

    glProgramUniform4fv(program->getProgramID(), globalAmbLoc, 1, m_pGlobalAmt);
    glProgramUniform4fv(program->getProgramID(), lightAmbLoc, 1, m_pLightAmt);
    glProgramUniform4fv(program->getProgramID(), lightDiffLoc, 1, m_pLightDif);
    glProgramUniform4fv(program->getProgramID(), lightSpecLoc, 1, m_pLightSpe);
    glProgramUniform3fv(program->getProgramID(), lightPosLoc, 1, m_pLightPos);
    glProgramUniform4fv(program->getProgramID(), materialAmtLoc, 1, m_pBronzeAmt);
    glProgramUniform4fv(program->getProgramID(), materialDiffLoc, 1, m_pBronzeDif);
    glProgramUniform4fv(program->getProgramID(), materialSpecLoc, 1, m_pBronzeSpe);
    glProgramUniform1f(program->getProgramID(), materialShinLoc, m_BronzeShininess);
}

void GraphicsShadow::drawTexAtLight() {
    useProgram(m_pShadowProgram);

    glBindVertexArray(m_VAO[0]);

    glm::mat4 mMat = glm::translate(glm::mat4(1.0f), m_torusPos);
    mMat = glm::rotate(mMat, Utils::toRadians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 shadowMVP1 = m_lightPmatrix * m_lightVmatrix * mMat;
    GLuint shadowMVPLoc = m_pShadowProgram->getUniformLoc("shadowMVP");
    glUniformMatrix4fv(shadowMVPLoc, 1, GL_FALSE, glm::value_ptr(shadowMVP1));

    // draw the torus.
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[1]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO[4]);
    glDrawElements(GL_TRIANGLES, m_pTorus->getIndiceNum(), GL_UNSIGNED_INT, 0);

    // draw the pyramid.
    mMat = glm::translate(glm::mat4(1.0f), m_pyramidPos);
    mMat = glm::rotate(mMat, Utils::toRadians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    mMat = glm::rotate(mMat, Utils::toRadians(40.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    shadowMVP1 = m_lightPmatrix * m_lightVmatrix * mMat;
    glUniformMatrix4fv(shadowMVPLoc, 1, GL_FALSE, glm::value_ptr(shadowMVP1));

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glDrawArrays(GL_TRIANGLES, 0, m_pPyramid->getVerticeNum());
    glBindVertexArray(0);
}

void GraphicsShadow::drawContentAtCamera() {
    useProgram(m_pRenderProgram);

    glBindVertexArray(m_VAO[0]);

    GLuint mvLoc = m_pRenderProgram->getUniformLoc("mv_matrix");
    GLuint projLoc = m_pRenderProgram->getUniformLoc("proj_matrix");
    GLuint norLoc = m_pRenderProgram->getUniformLoc("norm_matrix");
    GLuint shadowMVPLoc = m_pRenderProgram->getUniformLoc("shadowMVP");

    glm::mat4 vMat = glm::translate(glm::mat4(1.0f), 
                                    glm::vec3(-m_cameraPos.x, -m_cameraPos.y, -m_cameraPos.z));

    // ������תЧ��
    vMat = glm::rotate(vMat, Utils::toRadians(m_dAngleDeltaX), glm::vec3(0.0, 1.0, 0.0));
    vMat = glm::rotate(vMat, Utils::toRadians(m_dAngleDeltaY), glm::vec3(1.0, 0.0, 0.0));

    // ��������Ч��
    //vMat = glm::scale(vMat, glm::vec3(5.0, 5.0, 5.0f));

    glm::mat4 mMat = glm::translate(glm::mat4(1.0f), m_torusPos);
    mMat = glm::rotate(mMat, Utils::toRadians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    m_curLightPos = glm::vec3(m_lightPos);
    installLight(m_pRenderProgram, vMat);

    glm::mat4 mvMat = vMat * mMat;
    m_invTrMat = glm::transpose(glm::inverse(mvMat));
    glm::mat4 shadowMVP2 = m_toShadowMat * m_lightPmatrix * m_lightVmatrix * mMat;

    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(m_pMat));
    glUniformMatrix4fv(norLoc, 1, GL_FALSE, glm::value_ptr(m_invTrMat));
    glUniformMatrix4fv(shadowMVPLoc, 1, GL_FALSE, glm::value_ptr(shadowMVP2));

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[1]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[3]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO[4]);
    glDrawElements(GL_TRIANGLES, m_pTorus->getIndiceNum(), GL_UNSIGNED_INT, 0);

    mMat = glm::translate(glm::mat4(1.0f), m_pyramidPos);
    mMat = glm::rotate(mMat, Utils::toRadians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    mMat = glm::rotate(mMat, Utils::toRadians(40.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    m_curLightPos = glm::vec3(m_lightPos);
    installLight(m_pRenderProgram, vMat);

    mvMat = vMat * mMat;
    m_invTrMat = glm::transpose(glm::inverse(mvMat));
    shadowMVP2 = m_toShadowMat * m_lightPmatrix * m_lightVmatrix * mMat;

    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(m_pMat));
    glUniformMatrix4fv(norLoc, 1, GL_FALSE, glm::value_ptr(m_invTrMat));
    glUniformMatrix4fv(shadowMVPLoc, 1, GL_FALSE, glm::value_ptr(shadowMVP2));

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[2]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, m_pPyramid->getVerticeNum());

    glBindVertexArray(0);
}

void GraphicsShadow::drawForSelect() {
    useProgram(m_pRenderProgram);

    glBindVertexArray(m_VAO[0]);

    GLuint selectLoc = m_pRenderProgram->getUniformLoc("inSelected");
    glUniform1i(selectLoc, 1);

    glPushName(1);
    GLuint mvLoc = m_pRenderProgram->getUniformLoc("mv_matrix");
    GLuint projLoc = m_pRenderProgram->getUniformLoc("proj_matrix");
    GLuint norLoc = m_pRenderProgram->getUniformLoc("norm_matrix");
    GLuint shadowMVPLoc = m_pRenderProgram->getUniformLoc("shadowMVP");

    glm::mat4 vMat = glm::translate(glm::mat4(1.0f),
                                    glm::vec3(-m_cameraPos.x, -m_cameraPos.y, -m_cameraPos.z));

    // ������תЧ��
    vMat = glm::rotate(vMat, Utils::toRadians(m_dAngleDeltaX), glm::vec3(0.0, 1.0, 0.0));
    vMat = glm::rotate(vMat, Utils::toRadians(m_dAngleDeltaY), glm::vec3(1.0, 0.0, 0.0));

    // ��������Ч��
    //vMat = glm::scale(vMat, glm::vec3(5.0, 5.0, 5.0f));

    glm::mat4 mMat = glm::translate(glm::mat4(1.0f), m_torusPos);
    mMat = glm::rotate(mMat, Utils::toRadians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    m_curLightPos = glm::vec3(m_lightPos);
    installLight(m_pRenderProgram, vMat);

    glm::mat4 mvMat = vMat * mMat;
    m_invTrMat = glm::transpose(glm::inverse(mvMat));
    glm::mat4 shadowMVP2 = m_toShadowMat * m_lightPmatrix * m_lightVmatrix * mMat;

    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(m_pMat));
    glUniformMatrix4fv(norLoc, 1, GL_FALSE, glm::value_ptr(m_invTrMat));
    glUniformMatrix4fv(shadowMVPLoc, 1, GL_FALSE, glm::value_ptr(shadowMVP2));

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[1]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[3]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO[4]);
    glDrawElements(GL_TRIANGLES, m_pTorus->getIndiceNum(), GL_UNSIGNED_INT, 0);
    glPopName();

    glPushName(2);
    mMat = glm::translate(glm::mat4(1.0f), m_pyramidPos);
    mMat = glm::rotate(mMat, Utils::toRadians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    mMat = glm::rotate(mMat, Utils::toRadians(40.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    m_curLightPos = glm::vec3(m_lightPos);
    installLight(m_pRenderProgram, vMat);

    mvMat = vMat * mMat;
    m_invTrMat = glm::transpose(glm::inverse(mvMat));
    shadowMVP2 = m_toShadowMat * m_lightPmatrix * m_lightVmatrix * mMat;

    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(m_pMat));
    glUniformMatrix4fv(norLoc, 1, GL_FALSE, glm::value_ptr(m_invTrMat));
    glUniformMatrix4fv(shadowMVPLoc, 1, GL_FALSE, glm::value_ptr(shadowMVP2));

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[2]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, m_pPyramid->getVerticeNum());
    glPopName();

    glBindVertexArray(0);
}

void GraphicsShadow::modifyCameraPos(glm::vec3 &deltaPos) {
    m_cameraPos = glm::vec3(m_cameraPos - deltaPos);
}

void GraphicsShadow::selectedItem() {
    GLuint pickBuffer[512];
    GLint nPicks, vp[4];

    glGetIntegerv(GL_VIEWPORT, vp);
    glSelectBuffer(512, pickBuffer);

    glRenderMode(GL_SELECT);
    glInitNames();
    glPushName(-1);

    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    std::array<double, 2> mousePos = getMouseOriginPos();//getMouseLButtonClickPos();
    gluPickMatrix((GLdouble)mousePos[0], (GLdouble)(vp[3] - mousePos[1]), 5.0, 5.0, vp);
    gluPerspective(Utils::toRadians(90), 1.0, 0.1, 1000);
    glMatrixMode(GL_MODELVIEW);
    //gluOrtho2D(0, m_iWinWidth, 0.0, m_iWinHeight);
    //glOrtho(0.0, 5.0, 5.0, 10.0, 0.1, 1000);
    //glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    
    drawForSelect();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glFlush();

    nPicks = glRenderMode(GL_RENDER);
    if (nPicks == 0) {
        Utils::outputLog("there is no item selected");
    } else {
        std::string log = Utils::formatString("Selected {0} item", nPicks);
        Utils::outputLog(log);
    }
}
