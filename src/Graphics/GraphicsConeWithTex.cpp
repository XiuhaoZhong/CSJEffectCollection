#include "GraphicsConeWithTex.h"

// 三角锥中的纹理坐标;
float g_textureCoordinates[36] = {
    0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
    0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
    0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
    0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f
};

GraphicsConeWithTex::GraphicsConeWithTex() {

}

GraphicsConeWithTex::~GraphicsConeWithTex() {

}

bool GraphicsConeWithTex::init(GLFWwindow * window) {
    std::string verPath = Utils::getResourcePath("shaders\\ConeWithTex\\vertexShader.glsl");
    std::string fragPath = Utils::getResourcePath("shaders\\ConeWithTex\\fragmentShader.glsl");
    if (!createShader(verPath, fragPath)) {
        // create shader failed.
        return false;
    }

    m_cameraX = 0.0f;
    m_cameraY = 0.0f;
    m_cameraZ = 4.0f;

    m_pyrLocX = 0.0f;
    m_pyrLocY = 0.0f;
    m_pyrLocZ = 0.0f;

    glGenVertexArrays(1, m_vVao);
    glBindVertexArray(m_vVao[0]);
    glGenBuffers(numVBOs, m_vVbo);

    glBindBuffer(GL_ARRAY_BUFFER, m_vVbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_PyramidPositions), g_PyramidPositions, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_vVbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_textureCoordinates), g_textureCoordinates, GL_STATIC_DRAW);
    glBindVertexArray(0); 
    // 此处犯过一个错误，giBindVertexArray(0)之后，在display中没有重新执行glBindVertexArray(m_vVao[0])，
    // 导致没有绘制任何东西，是通过glGetError()方法来排查问题的，返回的错误码是1282 2024/03/25.

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float aspect = (float)width / (float)height;
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);

    // 此处调用的SOIL中的loadTexture方法，直接报错非法内存错误。
    //m_brickTex = Utils::loadTextureWithFile(Utils::getResourcePath("\\resources\\brick1.jpg").c_str());

    glGenTextures(1, &m_brickTex);
    glBindTexture(GL_TEXTURE_2D, m_brickTex);
    int tride;
    std::string imgPath = Utils::getResourcePath("\\resources\\brick1.jpg");
    unsigned char *image = SOIL_load_image(imgPath.c_str(), &width, &height, &tride, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenTextures(1, &m_brickArrowTex);
    glBindTexture(GL_TEXTURE_2D, m_brickArrowTex);
    std::string arrImgPath = Utils::getResourcePath("\\resources\\brick1upArrow.jpg");
    unsigned char *arr_image = SOIL_load_image(arrImgPath.c_str(), &width, &height, &tride, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, arr_image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    if (glewIsSupported("GL_EXT_texture_filter_anisotropic")) {
        // 各向异性过滤(AF)，标准的纹理采样时正方形区域采样，在有倾斜的情况时，
        // 会损失一些细节
        // AF是采用长方形的区域采样，能保留更多的细节，这属于OpenGL的扩展，
        // 效率可能比标准采样低一点
        // 在当前的环境中，效果很好，可以在工程中的Textures文件夹中查看对比效果
        GLfloat anisoSetting = 0.0f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoSetting);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoSetting);
    }
    SOIL_free_image_data(arr_image);
    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

void GraphicsConeWithTex::display(GLFWwindow * window, double currentTime) {
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    useShaders();

    GLuint mvLoc = glGetUniformLocation(m_vRenderingProgram, "mv_matrix");
    GLuint projLoc = glGetUniformLocation(m_vRenderingProgram, "proj_matrix");

    vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-m_cameraX, -m_cameraY, -m_cameraZ));

    mMat = glm::translate(glm::mat4(1.0f), glm::vec3(m_pyrLocX, m_pyrLocY, m_pyrLocZ));

    mMat = glm::rotate(mMat, -0.45f, glm::vec3(1.0f, 0.0f, 0.0f));
    mMat = glm::rotate(mMat, 0.61f, glm::vec3(0.0f, 1.0f, 0.0f));
    mMat = glm::rotate(mMat, 0.00f, glm::vec3(0.0f, 0.0f, 1.0f));

    mvMat = vMat * mMat;

    // 设置uniform变量的值
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

    glBindVertexArray(m_vVao[0]);
    glBindBuffer(GL_ARRAY_BUFFER, m_vVbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, m_vVbo[1]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_brickTex);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glDrawArrays(GL_TRIANGLES, 0, 18);

    glBindVertexArray(0);
}
