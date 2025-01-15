#pragma once

#include "GraphicsBase.h"

class GraphicPoint : public GraphicBase {
public:
	GraphicPoint();
	~GraphicPoint();

	virtual bool init(GLFWwindow *window) override;

	virtual void display(GLFWwindow *window, double currentTime) override;

private:

	float pointSize;
	float incSize;

};

