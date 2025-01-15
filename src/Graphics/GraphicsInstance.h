#pragma once

#include "GraphicsBase.h"

class GraphicInstance : public GraphicBase {
public:
	GraphicInstance();
	~GraphicInstance();

	bool init(GLFWwindow *window) override;

	void display(GLFWwindow *window, double currentTime) override;

private:
	float m_vCameraX;
	float m_vCameraY;
	float m_vCameraZ;

	glm::mat4 m_vPMat;

	GLuint m_vVao[numVAOs];
	GLuint m_vVbo[numVBOs];

};

