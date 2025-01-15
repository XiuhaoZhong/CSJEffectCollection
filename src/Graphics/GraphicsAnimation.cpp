#include "GraphicsAnimation.h"

using std::string;

GraphicAnimation::GraphicAnimation() {
	curX = 0.0f;
	incX = 0.01f;
}

GraphicAnimation::~GraphicAnimation() {

}

bool GraphicAnimation::init(GLFWwindow *window) {
	string verPath = Utils::getResourcePath("shaders\\animationShaders\\vertexShader.glsl");
	string fragPath = Utils::getResourcePath("shaders\\animationShaders\\fragmentShader.glsl");

	bool createShaderSuccess = this->createShader(verPath, fragPath);
	if (!createShaderSuccess) {
		return false;
	}

	GLuint vao[numVAOs];
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);

	return true;
}

void GraphicAnimation::display(GLFWwindow *window, double currentTime) {
	curX += incX;

	if (curX > 1.5f) {
		incX = -0.01f;
	}

	if (curX < -1.5f) {
		incX = 0.01f;
	}

	GLuint offsetLoc = glGetUniformLocation(m_vRenderingProgram, "offset");
	glProgramUniform1f(m_vRenderingProgram, offsetLoc, curX);
	// 设置下面这行，表示绘制的无填充的线框模型;
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}