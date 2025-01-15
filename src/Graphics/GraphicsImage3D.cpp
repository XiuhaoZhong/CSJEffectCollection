#include "GraphicsImage3D.h"

glm::vec3 g_cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

GraphicImage3D::GraphicImage3D() {

}

GraphicImage3D::~GraphicImage3D() {
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
}

bool GraphicImage3D::init(GLFWwindow *window) {
	string verPath = Utils::getResourcePath("shaders\\Image3D\\vertexShader.glsl");
	string fragPath = Utils::getResourcePath("shaders\\Image3D\\fragmentShader.glsl");

	bool createShaderSuccess = this->createShader(verPath, fragPath);
	if (!createShaderSuccess) {
		return false;
	}

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex3D), g_vertex3D, GL_STATIC_DRAW);

	glBufferData(GL_ARRAY_BUFFER, sizeof(g_cubicPoints), g_cubicPoints, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_rectPosIdx), g_rectPosIdx, GL_STATIC_DRAW);

	//Position attribute;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);

	glGenTextures(1, &m_Texture1);
	glBindTexture(GL_TEXTURE_2D, m_Texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;

	const GLchar *containerImg = Utils::getResourcePath("\\resources\\container.jpg").c_str();
	unsigned char *image = SOIL_load_image(containerImg, &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &m_Texture2);
	glBindTexture(GL_TEXTURE_2D, m_Texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	containerImg = Utils::getResourcePath("\\resources\\awesomeface.png").c_str();
	image = SOIL_load_image(containerImg, &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

void GraphicImage3D::display(GLFWwindow *window, double currentTime) {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	useShaders();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_Texture1);
	glUniform1i(glGetUniformLocation(m_vRenderingProgram, "ourTexture1"), 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_Texture2);
	glUniform1i(glGetUniformLocation(m_vRenderingProgram, "ourTexture2"), 1);

	glm::mat4 trans(1.0f);
	glm::mat4 model(1.0f);
	glm::mat4 view(1.0f);
	glm::mat4 projection(1.0f);
	model = glm::rotate(model, (GLfloat)glfwGetTime() * glm::radians(90.0f), glm::vec3(0.5f, 1.0f, 0.2f));
	//model = glm::rotate(model, 45.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::translate(view, glm::vec3(0.0f, 0.f, -3.0f));
	projection = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);

	GLint modelLoc = glGetUniformLocation(m_vRenderingProgram, "model");
	GLint viewLoc = glGetUniformLocation(m_vRenderingProgram, "view");
	GLint projLoc = glGetUniformLocation(m_vRenderingProgram, "projection");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glBindVertexArray(m_VAO);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	// 绘制多个正方体;
	//glBindVertexArray(m_VAO);
	//for (GLuint i = 0; i < 10; i++) {
	//	glm::mat4 model(1.0f);
	//	model = glm::translate(model, g_cubePositions[i]);
	//	GLfloat angle = 20.0f * i;
	//	model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
	//	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	//	glDrawArrays(GL_TRIANGLES, 0, 36);
	//}
	glBindVertexArray(0);
}