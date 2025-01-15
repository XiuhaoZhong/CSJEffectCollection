#pragma once

#include <vector>

#include "GraphicsBase.h"
#include "GraphicShapeBase.h"

class GraphicsSphere : public GraphicBase {
public:
    GraphicsSphere();
    ~GraphicsSphere();

    bool init(GLFWwindow *window) override;
    void display(GLFWwindow *window, double currentTime) override;

protected:
    bool initPointsData();

private:
    float m_cameraX;
    float m_cameraY;
    float m_cameraZ;

    float m_sphLocX;
    float m_sphLocY;
    float m_sphLocZ;

    float m_rotAmt = 0.0f;
    GLuint m_vao[1];
    GLuint m_vbo[3];
    GLuint m_earthTex;

    GLuint m_mvLoc;
    GLuint m_projLoc;

    glm::mat4 m_pMat;
    glm::mat4 m_vMat;
    glm::mat4 m_mMat;
    glm::mat4 m_mvMat;

    GraphicSpShape m_shape = nullptr;
};

