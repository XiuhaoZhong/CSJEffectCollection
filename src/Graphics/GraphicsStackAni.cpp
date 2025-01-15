#include "GraphicsStackAni.h"

GraphicStackAni::GraphicStackAni() {
	m_vCameraX = 0.0;
	m_vCameraY = 0.0;
	m_vCameraZ = 0.0;

	m_pMvStack = nullptr;
}

GraphicStackAni::~GraphicStackAni() {
	if (m_pMvStack) {
		delete m_pMvStack;
		m_pMvStack = nullptr;
	}
}

bool GraphicStackAni::init(GLFWwindow *window) {
	string verPath = Utils::getResourcePath("shaders\\StackGraphicShaders\\vertexShader.glsl");
	string fragPath = Utils::getResourcePath("shaders\\StackGraphicShaders\\fragmentShader.glsl");

	bool createShaderSuccess = createShader(verPath, fragPath);
	if (!createShaderSuccess) {
		return false;
	}	

	glGenVertexArrays(numVAOs, m_vVao);
	glBindVertexArray(m_vVao[0]);
	glGenBuffers(numVBOs, m_vVbo);

	glBindBuffer(GL_ARRAY_BUFFER, m_vVbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertexPositions), g_vertexPositions, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vVbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_PyramidPositions), g_PyramidPositions, GL_STATIC_DRAW);

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	float aspect = (float)width / (float)height;
	m_vPmat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);

	m_vCameraX = 0.0f;
	m_vCameraY = 0.0f;
	m_vCameraZ = 12.0f;

	return true;
}

void GraphicStackAni::display(GLFWwindow *window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	//glUseProgram(m_vRenderingProgram);

	GLuint mvLoc = glGetUniformLocation(m_vRenderingProgram, "mv_matrix");
	GLuint projLoc = glGetUniformLocation(m_vRenderingProgram, "proj_matrix");
	
	if (!m_pMvStack) {
		m_pMvStack = new std::stack<glm::mat4>;
	}

	m_vVmat = glm::translate(glm::mat4(1.0f), glm::vec3(-m_vCameraX, -m_vCameraY, -m_vCameraZ));
	m_pMvStack->push(m_vVmat);

	glUniformMatrix4fv(projLoc, 1, GL_FALSE,  glm::value_ptr(m_vPmat));

	// -------------------------------- pyramid == sun----------------------------------
	m_pMvStack->push(m_pMvStack->top());
	m_pMvStack->top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	m_pMvStack->push(m_pMvStack->top());
	m_pMvStack->top() *= rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(m_pMvStack->top()));
	glBindBuffer(GL_ARRAY_BUFFER, m_vVbo[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 18);
	m_pMvStack->pop();

	// -------------------------------- cube == planet ----------------------------------
	m_pMvStack->push(m_pMvStack->top());
	m_pMvStack->top() *= glm::translate(glm::mat4(1.0f), glm::vec3(sin((float)currentTime) * 4.0, 0.0f, cos((float)currentTime) * 4.0f));
	m_pMvStack->push(m_pMvStack->top());
	m_pMvStack->top() *= rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(m_pMvStack->top()));
	glBindBuffer(GL_ARRAY_BUFFER, m_vVbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	m_pMvStack->pop();

	// -------------------------------- small cube == moon ----------------------------------
	m_pMvStack->push(m_pMvStack->top());
	m_pMvStack->top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, sin((float)currentTime) * 2.0f, cos((float)currentTime) * 2.0f));
	m_pMvStack->top() *= rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0f, 0.0f, 1.0f));
	m_pMvStack->top() *= scale(glm::mat4(1.0f), glm::vec3(0.25f, 0.25f, 0.25f));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(m_pMvStack->top()));
	glBindBuffer(GL_ARRAY_BUFFER, m_vVbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	m_pMvStack->pop();
	m_pMvStack->pop();
	m_pMvStack->pop();
	m_pMvStack->pop();

}