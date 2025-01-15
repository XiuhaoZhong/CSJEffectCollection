#pragma once

#include "GraphicsBase.h"

class GraphicImage3D : public GraphicBase {
public:
	GraphicImage3D();
	~GraphicImage3D();

	bool init(GLFWwindow *window) override;
	void display(GLFWwindow *window, double currentTime) override;

private:
	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_EBO;

	GLuint m_Texture1;
	GLuint m_Texture2;
};

