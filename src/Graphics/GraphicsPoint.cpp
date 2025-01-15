#include "GraphicsPoint.h"

#include <string>

using namespace std;

GraphicPoint::GraphicPoint() {
	pointSize = 30.0f;
	incSize = 1.0f;
}

GraphicPoint::~GraphicPoint() {

}

bool GraphicPoint::init(GLFWwindow *window) {
	string verPath = Utils::getResourcePath("shaders\\pointShaders\\vertexShader.glsl");
	string fragPath = Utils::getResourcePath("shaders\\pointShaders\\fragmentShader.glsl");

	bool createShaderSuccess = this->createShader(verPath, fragPath);
	if (!createShaderSuccess) {
		return false;
	}

	GLuint vao[numVAOs];
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);

	return true;
}

void GraphicPoint::display(GLFWwindow *window, double currentTime) {
	this->useShaders();

	if (pointSize > 50) {
		incSize = -1.0f;
	}

	if (pointSize < 10) {
		incSize = 1.0f;
	}

	pointSize += incSize;
	glPointSize(pointSize);
	glDrawArrays(GL_POINTS, 0, 1);
}