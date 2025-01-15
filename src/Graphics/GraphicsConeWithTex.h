#pragma once

#include "GraphicsBase.h"

//#include "glm\glm.hpp"
//#include "glm\gtc\type_ptr.hpp"
//#include "glm\gtc\matrix_transform.hpp"

// 在三角锥体表面绘制图片纹理;
class GraphicsConeWithTex : public GraphicBase {
public:
    GraphicsConeWithTex();
    ~GraphicsConeWithTex();

    bool init(GLFWwindow *window) override;

    void display(GLFWwindow *window, double currentTime) override;

private:
    // 摄像机的位置;
    float m_cameraX;
    float m_cameraY;
    float m_cameraZ;

    // 三角锥的位置;
    float m_pyrLocX;
    float m_pyrLocY;
    float m_pyrLocZ;

    // 变换的矩阵;
    glm::mat4 pMat; // 透视矩阵;
    glm::mat4 vMat;
    glm::mat4 mMat;
    glm::mat4 mvMat;

    GLuint m_vVao[numVAOs];
    GLuint m_vVbo[numVBOs];

    // 图片纹理;
    GLuint m_brickTex;
    GLuint m_brickArrowTex;

    // 窗口大小;
    int width;
    int height;


};

