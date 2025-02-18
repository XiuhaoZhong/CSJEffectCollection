#ifndef __GRAPHICSHADOW_H__
#define __GRAPHICSHADOW_H__

#include "GraphicsBase.h"

#include "GraphicProgram.h"
#include "GraphicShapeBase.h"

constexpr int ShadowNumVAO = 1;
constexpr int ShadowNumVBO = 5;

class GraphicsShadow : public GraphicsBase {
public:
    GraphicsShadow();
    ~GraphicsShadow();

    bool init(GLFWwindow* window) override;
    void display(GLFWwindow* window, double currentTime) override;

protected:
    void onMouseLButtonPressed() override;
    void onMouseMove(double xCoord, double yCoord) override;
    void onMouseScroll(double xOffset, double yOffset) override;
    void onMouseLButtonReleased() override;
    void onResize(int width, int height) override;

    void initShapes();
    void setupLightsAndMaterial();
    void setupShadowTex(int width, int height);
    void installLight(GraphicSpProgram &program, glm::mat4 vMat);

    void drawTexAtLight();
    void drawContentAtCamera();

    void drawForSelect();

    void modifyCameraPos(glm::vec3 &deltaPos);

    void selectedItem();

private:
    int m_iWinWidth;
    int m_iWinHeight;

    GraphicSpShape m_pPyramid;
    GraphicSpShape m_pTorus;

    GraphicSpProgram m_pShadowProgram;
    GraphicSpProgram m_pRenderProgram;
    GraphicSpProgram m_pSelectProgram;

    glm::vec3 m_curLightPos;
    glm::vec3 m_transformed;

    glm::vec3 m_torusPos;
    glm::vec3 m_pyramidPos;
    glm::vec3 m_cameraPos;
    glm::vec3 m_lightPos;
    glm::vec3 m_originPos;
    glm::vec3 m_upDirection;

    glm::mat4 m_invTrMat;
    glm::mat4 m_pMat; 
    glm::mat4 m_toShadowMat;

    glm::mat4 m_lightVmatrix;
    glm::mat4 m_lightPmatrix;

    float *m_pGlobalAmt;
    float *m_pLightAmt;
    float *m_pLightDif;
    float *m_pLightSpe;

    float *m_pBronzeAmt;
    float *m_pBronzeDif;
    float *m_pBronzeSpe;
    float  m_BronzeShininess;

    float m_pLightPos[3];

    GLuint m_VAO[ShadowNumVAO];
    GLuint m_VBO[ShadowNumVBO];

    GLuint m_shadowBuffer;
    GLuint m_shadowTex;

    double m_dAngleDeltaX = 0.0;
    double m_dAngleDeltaY = 0.0;

};

#endif // __GRAPHICSHADOW_H__

