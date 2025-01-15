#include "GraphicsRectangle.h"

GraphicRectangle::GraphicRectangle() {

}

GraphicRectangle::~GraphicRectangle() {

}

bool GraphicRectangle::init(GLFWwindow *window) {
	string verPath = Utils::getResourcePath("shaders\\rectangle\\vertexShader.glsl");
	string fragPath = Utils::getResourcePath("shaders\\rectangle\\fragmentShader.glsl");

	bool createShaderSuccess = this->createShader(verPath, fragPath);
	if (!createShaderSuccess) {
		return false;
	}

	glGenVertexArrays(1, &vAO);
	glGenBuffers(1, &vBO);
	glGenBuffers(1, &eBO);

	glBindVertexArray(vAO);
	glBindBuffer(GL_ARRAY_BUFFER, vBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_rectPoints), g_rectPoints, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_rectPosIdx), g_rectPosIdx, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	
	glBindVertexArray(0);
	return true;
}

void GraphicRectangle::display(GLFWwindow *window, double currentTime) {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	useShaders();
	glBindVertexArray(vAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}