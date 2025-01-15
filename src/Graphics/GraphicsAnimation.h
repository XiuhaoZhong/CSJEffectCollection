#pragma once

#include "GraphicsBase.h"

class GraphicAnimation : public GraphicBase {
public:
	GraphicAnimation();
	~GraphicAnimation();

	virtual bool init(GLFWwindow *window) override;
	virtual void display(GLFWwindow *window, double currentTime) override;

private:
	float curX;
	float incX;
};

