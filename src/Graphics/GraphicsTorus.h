#pragma once

#include "GraphicsBase.h"

#include <vector>

#include "GraphicProgram.h"
#include "GraphicShapeBase.h"

constexpr int TorNumVAO = 1;
constexpr int TorNumVBO = 4;

typedef enum {
    GraphicTorus_Torus = 0,
    GraphicTorus_Gouraud,
    GraphicTorus_Phong,
    GraphicTorus_BlinnPhong
} GraphicTorusRenderType;

class GraphicTorus : public GraphicsBase {
public:
    GraphicTorus();
    ~GraphicTorus();

    bool init(GLFWwindow* window) override;
    void display(GLFWwindow* window, double currentTime) override;

protected:
    void initTorusProgram();
    void displayTorus(GLFWwindow* window, double currentTime);

    // Render with Gouraud light shadinge.
    void initGouraudProgram();
    void displayWithLight(GLFWwindow* window, double currentTime);
    void installGouraudLight(glm::mat4 vMatrix);

    // Render with phong light shading.
    void initPhongProgram();
    void initBlinnPhongProgram();

private:
    //int m_iNumVertices;
    //int m_iNumIndices;
    //int m_iPrec;

    GraphicSpShape m_shape = nullptr;

    //std::vector<int> m_vIndices;
    //std::vector<glm::vec3> m_vVertices;
    //std::vector<glm::vec2> m_vTexCoords;
    //std::vector<glm::vec3> m_vNormals;
    //std::vector<glm::vec3> m_vSTangents;
    //std::vector<glm::vec3> m_vTTangents;

    // Camera's position;
    float m_fCameraX;
    float m_fCameraY;
    float m_fCameraZ;

    // Toru's position;
    float m_fTorLocX;
    float m_fTorLocY;
    float m_fTorLocZ;

    // Light's position;
    float m_fLightLocX;
    float m_fLightLocY;
    float m_fLightLocZ;

    float m_vLightPos[3];
    glm::vec3 m_vCurLightPos;
    glm::vec3 m_vCurtransformed;

    // Light's rotate amt;
    float m_fLightAmt;

    // Uniform positions in program.
    GLuint m_globalLightAmbLoc;
    GLuint m_lightAmbLoc;
    GLuint m_lightDiffLoc;
    GLuint m_lightSpecLoc;
    GLuint m_lightPosLoc;
    GLuint m_materialAmbLoc;
    GLuint m_materialDiffLoc;
    GLuint m_materialSpenLoc;
    GLuint m_materialShinLoc;

    GLuint m_VAO[TorNumVAO];
    GLuint m_VBO[TorNumVBO];

    GLuint m_texBrick;
    float m_fRotAmt = 0.0f;

    glm::mat4 m_pMat;
    glm::mat4 m_vMat;
    glm::mat4 m_mMat;
    glm::mat4 m_mvMat;

    GLuint m_mvLoc;
    GLuint m_projLoc;
    GLuint m_norLoc;

    GraphicTorusRenderType m_renderType;

    GraphicSpProgram m_TorusProgram = nullptr;
    GraphicSpProgram m_gouraudProgram = nullptr;
    GraphicSpProgram m_phongProgram = nullptr;
    GraphicSpProgram m_blinnPhongProgram = nullptr;

    GraphicSpProgram m_curProgram = nullptr;
};

