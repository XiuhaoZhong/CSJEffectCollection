#include "GraphicsTorus.h"

#include "Utils/utils.h"

float globalAmbient[4] = { 0.7f, 0.7f, 0.7f, 1.0f };
float lightAmbient[4] = { 0.0f ,0.0f, 0.0f, 1.0f };
float lightDiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float lightSpecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

float* goldMatAmb = Utils::goldAmbient();
float* goldMatDif = Utils::goldDiffuse();
float* goldMatSpec = Utils::goldSpecular();
float  goldMatShininess = Utils::goldShininess();

GraphicTorus::GraphicTorus() {
    m_renderType = GraphicTorus_Torus;
    m_shape = GraphicShapeBase::createTorus(0.5f, 0.2f, 48);
}

GraphicTorus::~GraphicTorus() {

}

bool GraphicTorus::init(GLFWwindow* window) {

    switch (m_renderType) {
    case GraphicTorus_Torus:
        initTorusProgram();
        break;
    case GraphicTorus_Gouraud:
        initGouraudProgram();
        break;
    case GraphicTorus_Phong:
        initPhongProgram();
        break;
    case GraphicTorus_BlinnPhong:
        initBlinnPhongProgram();
        break;
    default:
        break;
    }

    m_fCameraX = 0.0f;
    m_fCameraY = 0.0f;
    m_fCameraZ = 2.0f;

    m_fTorLocX = 0.0f;
    m_fTorLocY = 0.0f;
    m_fTorLocZ = -0.5f;

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float aspect = (float)width / (float)height;
    m_pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);

    GraphicIntArray ind = m_shape->getIndices();
    int numVertices = m_shape->getVerticeNum();
    int numIndices = m_shape->getIndiceNum();
    GraphicVec3Array vertices = m_shape->getVertices();
    GraphicVec3Array normals = m_shape->getNormals();
    GraphicVec2Array texCoords = m_shape->getTexCoords();

    std::vector<float> pvalues;
    std::vector<float> tvalues;
    std::vector<float> nvalues;

    for (int i = 0; i < numVertices; i++) {
        pvalues.push_back(vertices[i].x);
        pvalues.push_back(vertices[i].y);
        pvalues.push_back(vertices[i].z);

        tvalues.push_back(texCoords[i].s);
        tvalues.push_back(texCoords[i].t);

        nvalues.push_back(normals[i].x);
        nvalues.push_back(normals[i].y);
        nvalues.push_back(normals[i].z);
    }

    glGenVertexArrays(TorNumVAO, m_VAO);
    glBindVertexArray(m_VAO[0]);

    glGenBuffers(TorNumVBO, m_VBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * 4, &ind[0], GL_STATIC_DRAW);

    glGenTextures(1, &m_texBrick);
    glBindTexture(GL_TEXTURE_2D, m_texBrick);
    int tride;
    std::string imgPath = Utils::getResourcePath("\\resources\\brick1.jpg");
    unsigned char* image = SOIL_load_image(imgPath.c_str(), &width, &height, &tride, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

void GraphicTorus::display(GLFWwindow* window, double currentTime) {
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    switch (m_renderType) {
    case GraphicTorus_Torus:
        displayTorus(window, currentTime);
        break;
    case GraphicTorus_Gouraud:
        useProgram(m_gouraudProgram);
        m_curProgram = m_gouraudProgram;
        displayWithLight(window, currentTime);
        break;
    case GraphicTorus_Phong:
        useProgram(m_phongProgram);
        m_curProgram = m_phongProgram;
        displayWithLight(window, currentTime);
        break;
    case GraphicTorus_BlinnPhong:
        useProgram(m_blinnPhongProgram);
        m_curProgram = m_blinnPhongProgram;
        displayWithLight(window, currentTime);
        break;
    default:
        break;
    }
}

void GraphicTorus::initTorusProgram() {
    string verPath = Utils::getResourcePath("shaders\\torusShaders\\vertexShader.glsl");
    string fragPath = Utils::getResourcePath("shaders\\torusShaders\\fragmentShader.glsl");
    

    m_TorusProgram = std::make_shared<GraphicProgram>(verPath, fragPath);
}

void GraphicTorus::displayTorus(GLFWwindow* window, double currentTime) {

    useProgram(m_TorusProgram);

    m_mvLoc = m_TorusProgram->getUniformLoc("mv_matrix");
    m_projLoc = m_TorusProgram->getUniformLoc("proj_matrix");

    m_vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-m_fCameraX, -m_fCameraY, -m_fCameraZ));
    m_mMat = glm::translate(glm::mat4(1.0f), glm::vec3(m_fTorLocX, m_fTorLocY, m_fTorLocZ));
    m_mMat = glm::rotate(m_mMat, Utils::toRadians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    m_mvMat = m_vMat * m_mMat;

    glUniformMatrix4fv(m_mvLoc, 1, GL_FALSE, glm::value_ptr(m_mvMat));
    glUniformMatrix4fv(m_projLoc, 1, GL_FALSE, glm::value_ptr(m_pMat));

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[1]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texBrick);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO[3]);
    glDrawElements(GL_TRIANGLES, m_shape->getIndiceNum(), GL_UNSIGNED_INT, 0);
}

void GraphicTorus::initGouraudProgram() {
    
    string verPath = Utils::getResourcePath("shaders\\torusShaders\\gouraudVertexShader.glsl");
    string fragPath = Utils::getResourcePath("shaders\\torusShaders\\gouraudFragmentShader.glsl");

    m_fLightLocX = 5.0f;
    m_fLightLocY = 2.0f;
    m_fLightLocZ = 2.0f;

    m_gouraudProgram = std::make_shared<GraphicProgram>(verPath, fragPath);
}

void GraphicTorus::displayWithLight(GLFWwindow* window, double currentTime) {
    m_mvLoc = m_curProgram->getUniformLoc("mv_matrix");
    m_projLoc = m_curProgram->getUniformLoc("proj_matrix");
    m_norLoc = m_curProgram->getUniformLoc("norm_matrix");

    m_vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-m_fCameraX, -m_fCameraY, -m_fCameraZ));
    m_mMat = glm::translate(glm::mat4(1.0f), glm::vec3(m_fTorLocX, m_fTorLocY, m_fTorLocZ));
    m_mMat *= glm::rotate(m_mMat, Utils::toRadians(35.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    m_vCurLightPos = glm::vec3(m_fLightLocX, m_fLightLocY, m_fLightLocZ);
    m_fLightAmt += 0.5f;
    glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), Utils::toRadians(m_fLightAmt), glm::vec3(0.0f, 0.0f, 1.0f));
    m_vCurLightPos = glm::vec3(rMat * glm::vec4(m_vCurLightPos, 1.0f));

    installGouraudLight(m_vMat);

    m_mvMat = m_vMat * m_mMat;
    glm::mat4 invTrMat = glm::transpose(glm::inverse(m_mvMat));

    glUniformMatrix4fv(m_mvLoc, 1, GL_FALSE, glm::value_ptr(m_mvMat));
    glUniformMatrix4fv(m_projLoc, 1, GL_FALSE, glm::value_ptr(m_pMat));
    glUniformMatrix4fv(m_norLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));

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

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO[3]);
    glDrawElements(GL_TRIANGLES, m_shape->getIndiceNum(), GL_UNSIGNED_INT, 0);
}

void GraphicTorus::installGouraudLight(glm::mat4 vMatrix) {
    glm::vec3 transformed = glm::vec3(vMatrix * glm::vec4(m_vCurLightPos, 1.0f));
 
    m_vLightPos[0] = transformed.x;
    m_vLightPos[1] = transformed.y;
    m_vLightPos[2] = transformed.z;

    // get the locations of the light and material fields in the shader.
    m_globalLightAmbLoc = m_curProgram->getUniformLoc("globalAmbient");
    m_lightAmbLoc = m_curProgram->getUniformLoc("light.ambient");
    m_lightDiffLoc = m_curProgram->getUniformLoc("light.diffuse");
    m_lightSpecLoc = m_curProgram->getUniformLoc("light.specular");
    m_lightPosLoc = m_curProgram->getUniformLoc("light.position");

    m_materialAmbLoc = m_curProgram->getUniformLoc("material.ambient");
    m_materialDiffLoc = m_curProgram->getUniformLoc("material.diffuse");
    m_materialSpenLoc = m_curProgram->getUniformLoc("material.specular");
    m_materialShinLoc = m_curProgram->getUniformLoc("material.shininess");

    // set the uniform light and material values in the shader.
    glProgramUniform4fv(m_curProgram->getProgramID(), m_globalLightAmbLoc, 1, globalAmbient);
    glProgramUniform4fv(m_curProgram->getProgramID(), m_lightAmbLoc, 1, lightAmbient);
    glProgramUniform4fv(m_curProgram->getProgramID(), m_lightDiffLoc, 1, lightDiffuse);
    glProgramUniform4fv(m_curProgram->getProgramID(), m_lightSpecLoc, 1, lightSpecular);
    glProgramUniform3fv(m_curProgram->getProgramID(), m_lightPosLoc, 1, m_vLightPos);

    glProgramUniform4fv(m_curProgram->getProgramID(), m_materialAmbLoc, 1, goldMatAmb);
    glProgramUniform4fv(m_curProgram->getProgramID(), m_materialDiffLoc, 1, goldMatDif);
    glProgramUniform4fv(m_curProgram->getProgramID(), m_materialSpenLoc, 1, goldMatSpec);
    glProgramUniform1f(m_curProgram->getProgramID(), m_materialShinLoc, goldMatShininess);
}

void GraphicTorus::initPhongProgram() {
    string verPath = Utils::getResourcePath("shaders\\torusShaders\\phongVertexShader.glsl");
    string fragPath = Utils::getResourcePath("shaders\\torusShaders\\phongFragmentShader.glsl");

    m_fLightLocX = 5.0f;
    m_fLightLocY = 2.0f;
    m_fLightLocZ = 2.0f;

    m_phongProgram = std::make_shared<GraphicProgram>(verPath, fragPath);
}

void GraphicTorus::initBlinnPhongProgram() {
    string verPath = Utils::getResourcePath("shaders\\torusShaders\\blinnPhongVertexShader.glsl");
    string fragPath = Utils::getResourcePath("shaders\\torusShaders\\blinnPhongFragmentShader.glsl");

    m_fLightLocX = 5.0f;
    m_fLightLocY = 2.0f;
    m_fLightLocZ = 2.0f;

    m_blinnPhongProgram = std::make_shared<GraphicProgram>(verPath, fragPath);
}
