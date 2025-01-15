#include "GraphicsTriangle.h"

GraphicTriagle::GraphicTriagle() {

}

GraphicTriagle::~GraphicTriagle() {

}

bool GraphicTriagle::init(GLFWwindow *window) {
	string verPath = Utils::getResourcePath("shaders\\triangleShaders\\vertexShader.glsl");
	string fragPath = Utils::getResourcePath("shaders\\triangleShaders\\fragmentShader.glsl");

	bool createShaderSuccess = this->createShader(verPath, fragPath);
	if (!createShaderSuccess) {
		return false;
	}

	GLuint vao[numVAOs];
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);

	return true;
}

void GraphicTriagle::display(GLFWwindow *window, double currentTime) {
	//glUseProgram(this->renderingProgram);

	glm::mat4 rotateVec = buildRotate(currentTime);

	GLuint curVec = glGetUniformLocation(m_vRenderingProgram, "rotateVec");
	glUniformMatrix4fv(curVec, 1, GL_FALSE, glm::value_ptr(rotateVec));

	glDrawArrays(GL_TRIANGLES, 0, 3);
}

glm::mat4 GraphicTriagle::buildRotate(double currentTime) {
	glm::mat4 rotateVec = glm::mat4(1.0);

	rotateVec = glm::rotate(rotateVec, (GLfloat)currentTime * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	return rotateVec;
}