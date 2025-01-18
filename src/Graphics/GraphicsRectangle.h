#pragma once

#include "GraphicsBase.h"

class GraphicRectangle : public GraphicsBase {
public:
	GraphicRectangle();
	~GraphicRectangle();

	bool init(GLFWwindow *window) override;
	void display(GLFWwindow *window, double currentTime) override;

private:

	GLuint vAO;
	GLuint vBO;
	GLuint eBO;
};

