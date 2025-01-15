#ifndef __GRAPHICSTRIANGLE_H__
#define __GRAPHICSTRIANGLE_H__

#include "GraphicsBase.h"

class GraphicTriagle : public GraphicBase {
public:
	GraphicTriagle();
	~GraphicTriagle();

	virtual bool init(GLFWwindow *window) override;

	virtual void display(GLFWwindow *window, double currentTime) override;

private:
	glm::mat4 buildRotate(double currentTime);

private:
	glm::mat4 rotateVec;

};

#endif // __GRAPHICSTRIANGLE_H__