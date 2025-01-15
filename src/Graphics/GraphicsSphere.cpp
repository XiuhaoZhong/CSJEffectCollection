#include "GraphicsSphere.h"

GraphicsSphere::GraphicsSphere() {
    m_shape = GraphicShapeBase::createSphere(48);
}

GraphicsSphere::~GraphicsSphere() {

}

bool GraphicsSphere::init(GLFWwindow * window) {

    string verPath = Utils::getResourcePath("shaders\\sphereShaders\\vertexShader.glsl");
    string fragPath = Utils::getResourcePath("shaders\\sphereShaders\\fragmentShader.glsl");

    bool createShaderSuccess = this->createShader(verPath, fragPath);
    if (!createShaderSuccess) {
        return false;
    }

    m_cameraX = 0;
    m_cameraY = 0;
    m_cameraZ = 2.0;

    m_sphLocX = 0;
    m_sphLocY = 0;
    m_sphLocZ = -1.0f;

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float aspect = (float)width / (float)height;
    m_pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);

    int numIndices = m_shape->getIndiceNum();
    GraphicIntArray indices = m_shape->getIndices();
    GraphicVec3Array vertexes = m_shape->getVertices();
    GraphicVec3Array normals = m_shape->getNormals();
    GraphicVec2Array texCoords = m_shape->getTexCoords();

    std::vector<float> pvalues;
    std::vector<float> tvalues;
    std::vector<float> nvalues;

    for (int i = 0; i < numIndices; i++) {
        pvalues.push_back((vertexes[indices[i]]).x);
        pvalues.push_back((vertexes[indices[i]]).y);
        pvalues.push_back((vertexes[indices[i]]).z);
        tvalues.push_back((texCoords[indices[i]]).s);
        tvalues.push_back((texCoords[indices[i]]).t);
        nvalues.push_back((normals[indices[i]]).x);
        nvalues.push_back((normals[indices[i]]).y);
        nvalues.push_back((normals[indices[i]]).z);
    }

    glGenVertexArrays(1, m_vao);
    glBindVertexArray(m_vao[0]);
    glGenBuffers(3, m_vbo);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);

    m_mvLoc = glGetUniformLocation(m_vRenderingProgram, "mv_matrix");
    m_projLoc = glGetUniformLocation(m_vRenderingProgram, "proj_matrix");

    glGenTextures(1, &m_earthTex);
    glBindTexture(GL_TEXTURE_2D, m_earthTex);
    int tride;
    std::string imgPath = Utils::getResourcePath("\\resources\\earth.jpg");
    unsigned char *image = SOIL_load_image(imgPath.c_str(), &width, &height, &tride, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

void GraphicsSphere::display(GLFWwindow * window, double currentTime) {
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    useShaders();

    m_vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-m_cameraX, -m_cameraY, -m_cameraZ));
    m_mMat = glm::translate(glm::mat4(1.0f), glm::vec3(m_sphLocX, m_sphLocY, m_sphLocZ));
    m_mvMat = m_vMat * m_mMat;

    glUniformMatrix4fv(m_mvLoc, 1, GL_FALSE, glm::value_ptr(m_mvMat));
    glUniformMatrix4fv(m_projLoc, 1, GL_FALSE, glm::value_ptr(m_pMat));

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_earthTex);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    glDrawArrays(GL_TRIANGLES, 0, m_shape->getIndiceNum());
}

