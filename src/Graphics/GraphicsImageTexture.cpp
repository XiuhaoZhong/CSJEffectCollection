#include "GraphicsImageTexture.h"

GraphicImageTexture::GraphicImageTexture() {

}

GraphicImageTexture::~GraphicImageTexture() {

}

bool GraphicImageTexture::init(GLFWwindow *window) {
	std::string verPath = Utils::getResourcePath("\\shaders\\imageShaders\\vertexShader.glsl");
	std::string fragPath = Utils::getResourcePath("\\shaders\\imageShaders\\fragmentShader.glsl");

	bool createShaderSuccess = this->createShader(verPath, fragPath);
	if (!createShaderSuccess) {
		std::cout << "" << std::endl;
		return false;
	}

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_image_vertices), g_image_vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_rectPosIdx), g_rectPosIdx, GL_STATIC_DRAW);

	// Position attributes;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	// color attributes;
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid *)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);

	// texCood attributes;
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid *)(6 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// generate the fist texture;
	glGenTextures(1, &m_Texture1);
	glBindTexture(GL_TEXTURE_2D, m_Texture1);
	// 设置超出纹理范围之外的处理方式，
	// GL_TEXTURE_WRAP_S，表示在x方向上超出范围，GL_REPEAT 表示重复使用该方向上的分量;
	// GL_TEXTURE_WRAP_T，表示在y方向上超出范围，GL_REPEAT 与上面一致;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	float borderColor[] = {1.0f, 1.0f , 1.0f, 1.0f};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// 设置纹理放大缩小的处理方法;
	// GL_TEXTURE_MIN_FILTER 表示缩小时的处理方式，GL_LINEAR 表示像素过渡采用线性方式处理;
	// GL_TEXTURE_MAG_FILTER 表示放大时的处理方式，GL_LINEAR 与上面一致;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	int width, height, tride;
	std::string containerImg = Utils::getResourcePath("\\resources\\container.jpg");
	unsigned char *image = SOIL_load_image(containerImg.c_str(), &width, &height, &tride, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// generate the second texture;
	glGenTextures(1, &m_Texture2);
	glBindTexture(GL_TEXTURE_2D, m_Texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	std::string faceImg = Utils::getResourcePath("\\resources\\awesomeface.png");
	image = SOIL_load_image(faceImg.c_str(), &width, &height, &tride, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

void GraphicImageTexture::display(GLFWwindow *window, double currentTime) {
	this->useShaders();

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_Texture1);
	glUniform1i(glGetUniformLocation(m_vRenderingProgram, "ourTexture1"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_Texture2);
	glUniform1i(glGetUniformLocation(m_vRenderingProgram, "ourTexture2"), 1);

	glm::mat4 trans(1.0f);
	trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
	trans = glm::rotate(trans, (GLfloat)glfwGetTime() * glm::radians(90.0f),glm::vec3(0.0, 0.0, 1.0));
	glUniformMatrix4fv(glGetUniformLocation(m_vRenderingProgram, "transform"), 1, GL_FALSE, glm::value_ptr(trans));

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	trans = glm::mat4(1.0f);
	GLfloat scaleAmount = abs(sin(glfwGetTime()));
	trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
	trans = glm::scale(trans, glm::vec3(scaleAmount, scaleAmount,scaleAmount));
	glUniformMatrix4fv(glGetUniformLocation(m_vRenderingProgram, "transform"), 1, GL_FALSE, glm::value_ptr(trans));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
