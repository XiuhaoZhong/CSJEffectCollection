#include "GraphicsInstance.h"

GraphicInstance::GraphicInstance() {
	m_vCameraX = 0.0;
	m_vCameraY = 0.0;
	m_vCameraZ = 0.0;
}

GraphicInstance::~GraphicInstance() {

}

bool GraphicInstance::init(GLFWwindow *window) {
	string verPath = Utils::getResourcePath("shaders\\instanceShaders\\vertexShader.glsl");
	string fragPath = Utils::getResourcePath("shaders\\instanceShaders\\fragmentShader.glsl");

	bool createShaderSuccess = this->createShader(verPath, fragPath);
	if (!createShaderSuccess) {
		return false;
	}

	GLuint vao[numVAOs];
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, m_vVbo);

	glBindBuffer(GL_ARRAY_BUFFER, m_vVbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertexPositions), g_vertexPositions, GL_STATIC_DRAW);

	m_vCameraX = 0.0f;
	m_vCameraY = 0.0f;
	m_vCameraZ = 420.0f;

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	float aspect = (float)width / (float)height;

	m_vPMat = glm::perspective(1.0472f, aspect, 1.0f, 1000.0f);

	return true;
}

void GraphicInstance::display(GLFWwindow *window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_vRenderingProgram);

	GLuint vLoc = glGetUniformLocation(m_vRenderingProgram, "v_matrix");
	GLuint projLoc = glGetUniformLocation(m_vRenderingProgram, "proj_matrix");

	glm::mat4 vMat = glm::translate(glm::mat4(1.0), glm::vec3(-m_vCameraX, -m_vCameraY, -m_vCameraZ));

	glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(vMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(m_vPMat));

	float timeFactor = (float)currentTime;
	GLuint tfLoc = glGetUniformLocation(m_vRenderingProgram, "tf");
	glUniform1f(tfLoc, (float)timeFactor);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 10000);
}