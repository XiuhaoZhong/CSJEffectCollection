#pragma once

#include "GraphicsBase.h"

class GraphicGradientTriangle : public GraphicBase {
public:
	GraphicGradientTriangle();
	~GraphicGradientTriangle();

	bool init(GLFWwindow *window) override;
	void display(GLFWwindow *window, double currentTime) override;

private:
	GLuint vAO;
	GLuint vBO;

};
