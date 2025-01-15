#pragma once

// GLEW_STATIC 的宏一定要加，表示使用的是glew的静态链接库，而且要先包含glew的头文件，再包含glfw的头文件！！！
#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL2\SOIL2.h>

#include "glm\glm.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_transform.hpp"
