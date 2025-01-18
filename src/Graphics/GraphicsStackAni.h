#pragma once

#include "GraphicsBase.h"

#include <Stack>

//#include "glm\glm.hpp"
//#include "glm\gtc\type_ptr.hpp"
//#include "glm\gtc\matrix_transform.hpp"

class GraphicStackAni : public GraphicsBase {
public:
	GraphicStackAni();
	~GraphicStackAni();

	bool init(GLFWwindow *window) override;
	void display(GLFWwindow *window, double currentTime) override;

private:
	float m_vCameraX;
	float m_vCameraY;
	float m_vCameraZ;

	glm::mat4 m_vPmat;
	glm::mat4 m_vVmat;
	glm::mat4 m_vMmat;
	glm::mat4 m_vMvMat;

	GLuint m_vVao[numVAOs];
	GLuint m_vVbo[numVBOs];

	std::stack<glm::mat4> *m_pMvStack;
};

