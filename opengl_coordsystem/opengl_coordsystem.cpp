#include <iostream>

#include <windows.h>

// GLEW ;
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW;
#include <GLFW/glfw3.h>

// Other Libs;
#include <SOIL.h>

// Other includes;
#include "Shader.h"

// Other glm
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow) {
	// Init GLFW;
	glfwInit();
	// Set all the required options for GLFW;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions;
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions;
	glfwSetKeyCallback(window, key_callback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions;
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers;
	glewInit();

	// Define the viewport dimensions;
	glViewport(0, 0, WIDTH, HEIGHT);

	// Build and compile our shader program;
	Shader shader("D:\\Professional\\Projects\\C++\\OpenGLLearning\\OpenGLDemos\\Debug\\coordsystem\\texture.vs", 
				  "D:\\Professional\\Projects\\C++\\OpenGLLearning\\OpenGLDemos\\Debug\\coordsystem\\texture.frag");

	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		// Positions			// Colors			// Texture Coords
		0.5f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f, // Top Right
		0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f, // Bottom Right
	   -0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f, // Bottom Left
	   -0.5f,  0.5f, 0.0f,		1.0f, 1.0f, 0.0f,	0.0f, 1.0f	// Top Left
	};

	GLuint indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	// Bind vertex data;
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Bind indices data;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attributes;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color attributes;
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// TexCoord attributes;
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	// Load and create a texture;
	GLuint texture;
	glGenTextures(1, &texture);
	// All upcoming GL_TEXTURE_2D operations now have effect on this texture object;
	glBindTexture(GL_TEXTURE_2D, texture);
	// Set the texture wrapping parameters;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// Set texture filtering parameters;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load image, create texture and generate mipmaps;
	int width, height, tride;
	unsigned char* image = SOIL_load_image("D:\\Professional\\Projects\\C++\\OpenGLLearning\\OpenGLDemos\\Debug\\container.jpg", &width, &height, &tride, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	// unBind texture when done, so we don't accidentily mess up our texture;
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint texture_face;
	glGenTextures(1, &texture_face);
	glBindTexture(GL_TEXTURE_2D, texture_face);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	image = SOIL_load_image("D:\\Professional\\Projects\\C++\\OpenGLLearning\\OpenGLDemos\\Debug\\awesomeface.png", &width, &height, &tride, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	glm::vec4 vecn(1.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 transn(1.0f);
	transn = glm::translate(transn, glm::vec3(1.0f, 1.0f, 0.0f));
	vecn = transn * vecn;
	std::cout << vecn.x << vecn.y << vecn.z << std::endl;

	// Game loop;
	while (!glfwWindowShouldClose(window)) {
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions;
		glfwPollEvents();

		// Render
		// Clear the colorbuffer;
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Bind Texture;
		//glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(shader.Program, "ourTexture1"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture_face);
		glUniform1i(glGetUniformLocation(shader.Program, "ourTexture2"), 1);

		// Activiate shader
		shader.Use();

		// When Using the glm::mat4 as a translate matrix, must declare as glm::mat trans(1.0)
		// this is a unit matrix, or the result is wrong matrix.
		glm::mat4 trans(1.0f);
		/*trans = glm::rotate(trans, 90.0f, glm::vec3(0.0, 0.0, 1.0));
		trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));*/
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, (GLfloat)glfwGetTime() * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		GLuint transformLoc = glGetUniformLocation(shader.Program, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		// Draw container;
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0);

		// Draw another container;
		trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
		GLfloat scaleAmount = abs(sin(glfwGetTime()));
		trans = glm::scale(trans, glm::vec3(scaleAmount, scaleAmount, scaleAmount));
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Swap the screen buffer;
		glfwSwapBuffers(window);
	}

	// Property de-allocate all resources once they've outlived thier purpose;
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	// terminate GLFW, clearing any resources allocated by GLFW;
	glfwTerminate();
	return 0;
}

// Is called whenever a key is pressed/released via GLFW;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}