#include "GraphicsGradientTriangle.h"

GraphicGradientTriangle::GraphicGradientTriangle() {

}

GraphicGradientTriangle::~GraphicGradientTriangle() {
	glDeleteVertexArrays(1, &vAO);
	glDeleteBuffers(1, &vBO);
}

bool GraphicGradientTriangle::init(GLFWwindow *window) {
	string verPath = Utils::getResourcePath("shaders\\gradientTriangle\\vertexShader.glsl");
	string fragPath = Utils::getResourcePath("shaders\\gradientTriangle\\fragmentShader.glsl");

	bool createShaderSuccess = this->createShader(verPath, fragPath);
	if (!createShaderSuccess) {
		return false;
	}

	glGenVertexArrays(1, &vAO);
	glGenBuffers(1, &vBO);

	glBindVertexArray(vAO);
	glBindBuffer(GL_ARRAY_BUFFER, vBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_gradientPts), g_gradientPts, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid *)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	
	return true;
}

void GraphicGradientTriangle::display(GLFWwindow *window, double currentTime) {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	useShaders();
	glBindVertexArray(vAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}