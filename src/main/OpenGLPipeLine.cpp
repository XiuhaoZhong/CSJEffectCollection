// OpenGLPipeLine.cpp : 定义应用程序的入口点。
//
#if _WIN32
#include <Windows.h>
#endif

#include <iostream>
#include <string>

#include "common/common.hpp"
#include "Graphics/GraphicsBase.h"

using namespace std;

std::weak_ptr<GraphicBase> gGraphicObj;

// mouse button click callback.
void mouse_callback(GLFWwindow *window, int button, int action, int mods);
// mouse move callbacl.
void mouse_move_callback(GLFWwindow *, double xCoord, double yCoord);
// mouse scroll callbacl.
void mouse_scroll_callback(GLFWwindow *window, double xOffset, double yOffset);
// Is called whenever a key is pressed/released via GLFW;
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

void framebuffer_size_change_callback(GLFWwindow *window, int width, int height);

int internalMain();

#if _WIN32
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) {
    int argc = 0;
    LPWSTR *argList;

    argList = CommandLineToArgvW(GetCommandLine(), &argc);

    return internalMain();
}
#elif __APPLE__
int main(int argc, char** argv) {
    // TODO: parse argc and argv if necessary.
    return internalMain();
}
#endif

int internalMain() {

	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow *window = glfwCreateWindow(800, 800, "OpenGL", NULL, NULL);
    if (!window) {
        std::cout << "Create window failed!" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
	glfwMakeContextCurrent(window);

	// Set the required callback functions;
	glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);
    glfwSetCursorPosCallback(window, mouse_move_callback);
    glfwSetScrollCallback(window, mouse_scroll_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_change_callback);

	if (glewInit() != GLEW_OK) {
		exit(EXIT_FAILURE);
	}
	glfwSwapInterval(1);

	GraphicSpObject graphic = GraphicBase::createGraphicObj(window, GraphicType_Shadow);
    if (!graphic) {
        exit(EXIT_FAILURE);
    }

	gGraphicObj = graphic;
    while (!glfwWindowShouldClose(window)) {

        glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 调用不同的graphic类的display方法，来实现具体的绘制;
        graphic->display(window, glfwGetTime());
        glfwSwapBuffers(window);

        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions;
        glfwPollEvents();
    }
	
	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}

void mouse_callback(GLFWwindow *window, int button, int action, int mods) {
    GraphicSpObject graphic = gGraphicObj.lock();
    if (!graphic) {
        return;
    }

    graphic->mouseButtonClicked(button, action, mods);
}

void mouse_move_callback(GLFWwindow *window, double xCoord, double yCoord) {
    GraphicSpObject graphic = gGraphicObj.lock();
    if (!graphic) {
        return;
    }

    graphic->mouseMove(xCoord, yCoord);
}

void mouse_scroll_callback(GLFWwindow *window, double xOffset, double yOffset) {
    GraphicSpObject graphic = gGraphicObj.lock();
    if (!graphic) {
        return;
    }

    graphic->mouseScroll(xOffset, yOffset);
}

// Is called whenever a key is pressed/released via GLFW;
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    GraphicSpObject graphic = gGraphicObj.lock();
    if (!graphic) {
        return;
    }


    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void framebuffer_size_change_callback(GLFWwindow *window, int width, int height) {
    GraphicSpObject graphic = gGraphicObj.lock();
    if (!graphic) {
        return;
    }

    graphic->framebufferSizeChanged(width, height);
}
