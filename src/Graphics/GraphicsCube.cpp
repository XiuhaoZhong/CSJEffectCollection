#include "GraphicsCube.h"

GraphicCube::GraphicCube() {
	m_vCameraX = 0.0;
	m_vCameraY = 0.0;
	m_vCameraZ = 0.0;

	m_vCubeLocX = 0.0;
	m_vCubeLocY = 0.0;
	m_vCubeLocZ = 0.0;

	m_vPyrLocX = 0.0;
	m_vPyrLocY = 0.0;
	m_vPyrLocZ = 0.0;
}

GraphicCube::~GraphicCube() {

}

bool GraphicCube::init(GLFWwindow *window) {
	string verPath = Utils::getResourcePath("shaders\\cubeShaders\\vertexShader.glsl");
	string fragPath = Utils::getResourcePath("shaders\\cubeShaders\\fragmentShader.glsl");

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

	glBindBuffer(GL_ARRAY_BUFFER, m_vVbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_PyramidPositions), g_PyramidPositions, GL_STATIC_DRAW);

	m_vCameraX = 0.0f;
	m_vCameraY = 0.0f;
	m_vCameraZ = 8.0f;

	m_vCubeLocX = 0.0f;
	m_vCubeLocY = -2.0f;
	m_vCubeLocZ = 0.0f;

	m_vPyrLocX = 2.0f;
	m_vPyrLocY = 2.0f;
	m_vPyrLocZ = 0.0f;

	return true;
}

void GraphicCube::display(GLFWwindow *window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);

	//glUseProgram(m_vRenderingProgram);

	GLuint mvLoc = glGetUniformLocation(m_vRenderingProgram, "mv_matrix");
	GLuint projLoc = glGetUniformLocation(m_vRenderingProgram, "proj_matrix");

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	float aspect = (float)width / (float)height;

	glm::mat4 pMat = glm::perspective(1.0472f, aspect, 1.0f, 1000.0f);

	glm::mat4 vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-m_vCameraX, -m_vCameraY, -m_vCameraZ));
	glm::mat4 mMat = glm::translate(glm::mat4(1.0f), glm::vec3(m_vCubeLocX, m_vCubeLocY, m_vCubeLocZ));

	/// ******** 随着时间进行对方块进行旋转 ********
	//glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), 1.75f * (float)currentTime, glm::vec3(0.0f, 1.0f, 0.0f));
	//rMat = glm::rotate(rMat, 1.75f * (float)currentTime, glm::vec3(1.0f, 0.0f, 0.0f));
	//rMat = glm::rotate(rMat, 1.75f * (float)currentTime, glm::vec3(0.0f, 0.0f, 1.0f));
	//glm::mat4 tMat = glm::translate(glm::mat4(1.0f), glm::vec3(sin(0.35f * currentTime) * 2.0f,
	//														   cos(0.52f * currentTime) * 2.0f,
	//														   sin(0.7f * currentTime) * 2.0f));
	//mMat = tMat * rMat;
	/// ******** 随着时间进行对方块进行旋转 ********

	glm::mat4 mvMat = vMat * mMat;

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	glBindBuffer(GL_ARRAY_BUFFER, m_vVbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	// 绘制金字塔立方体;
	mMat = glm::translate(glm::mat4(1.0), glm::vec3(m_vPyrLocX, m_vPyrLocY, m_vPyrLocZ));
	mvMat = vMat * mMat;

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	glBindBuffer(GL_ARRAY_BUFFER, m_vVbo[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 18);

}