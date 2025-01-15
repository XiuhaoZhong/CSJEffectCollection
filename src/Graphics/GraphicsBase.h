#pragma once

/*
 This class is base class for OpenGL Demos,

 It defines the createShader funcution to create shaders,
 and display function to draw the content.

 */

#include <cmath>

#include <iostream>
#include <string>
#include <memory>
#include <array>

#include "Utils/utils.h"
#include "Graphics/GraphicCommonData.h"
#include "Graphics/GraphicProgram.h"

enum GraphicType {
	GraphicType_Point = 0,			// 绘制一个点;
	GraphicType_triangle,			// 绘制一个旋转的三角形;
	GraphicType_rectangle,			// 绘制一个矩形;
	GraphicType_animation,			// 动画，左右移动的三角形;
	GraphicType_cube,				// 绘制一个正方体和三角锥;
	GraphicType_instance,			// 绘制无数个自由移动的正方体;
	GraphicType_StackAni,			// 利用栈来实现模拟地球、月亮、太阳转动的动画;
	GraphicType_GradientTriangle,	// 顶点三色渐变的三角形;	
	GraphicType_GraphicImage,		// 绘制图片生成的纹理;
	GraphicType_3DTexture,			// 绘制3D效果的纹理;
	GraphicType_camera,				// 实现摄像机视角;
    GraphicType_ConeWithTex,        // 在三角锥上绘制图片纹理;
    GraphicType_Sphere,             // 绘制球体;
	GraphicType_Torus,				// 绘制环面;
	GraphicType_Shadow,				// 阴影效果;

};

enum GraphicOperation {
	// Operations on the items in the scene(one item or several items).
	GraphicOperation_Move = 0,
	GraphicOperation_Scale,
	GraphicOperation_Rotate,
	// Operation on the whole scene.
	GraphicOperation_MoveScene,
	GraphicOperation_ScaleScene,
	GraphicOperation_RotateScene
};

using std::string;

const int numVAOs = 1;
const int numVBOs = 2;

class GraphicBase {
public:
	static std::shared_ptr<GraphicBase> createGraphicObj(GLFWwindow *window, GraphicType type);

	GraphicBase();
	virtual ~GraphicBase();

	virtual bool init(GLFWwindow *window) = 0;

	/*
	 create vertex shader and fragment shader.

	 @param vertexPath		vertex shader source path.
	 @param fragmentPath	fragment shader source path.

	 @return true, two shader create successfully, or return false.
	*/
	virtual bool createShader(string& vertexPath, string& fragmentPath);

	virtual void useShaders();

	virtual void useProgram(GraphicSpProgram program);

	/*
	 output the content which defined by the shaders.

	 @param window	display target.
	 @param currentTime display operation's time.

	*/
	virtual void display(GLFWwindow *window, double currentTime) = 0;

	// event handling
	void mouseMove(double xCoord, double yCoord);

	void mouseScroll(double xOffset, double yOffset);

	void framebufferSizeChanged(int width, int height);

	void mouseButtonClicked(int mouseBtn, int event, int modes);

	void setWindow(GLFWwindow *window) {
		m_pWindow = window;
	}

	glm::vec3 translateMousePos(double xCoord, double yCoord);

	std::array<double, 2> getMouseLButtonClickPos() {
		return { m_mousePrePos.x, m_mousePrePos.y };
	}

	std::array<double, 2> getMouseOriginPos() {
		return m_mouseOriginPos;
	}

protected:
	// event responses that subclass could implement.
	virtual void onMouseLButtonPressed();
	virtual void onMouseLButtonReleased();

	virtual void onMouseRButtonPressed();
	virtual void onMouseRButtonReleased();

	virtual void onMouseMButtonPressed();
	virtual void onMouseMButtonReleased();

	virtual void onMouseMove(double xCoord, double yCoord);

	virtual void onMouseScroll(double xOffset, double yOffset);

	virtual void onResize(int width, int height);

	/*
	 * @brief Get the mouse's delta position when it's moving, this function should only be 
	 *        used during the mouse left button pressed.
	 */
	glm::vec3 getMouseDeltaPos() {
		return m_mouseDeltaPos;
	}

	bool MouseLButtonPressed() const {
		return m_bMouseLButtonPressed;
	}

protected:
	GLuint m_vRenderingProgram;

	GLuint m_vVertexShader;
	GLuint m_vFragmentShader;

private:
	// GraphicBase just references an instance of the window, doesn't hold the instance,
	// so GrraphicBase doesn't manage windows's lifetime. Please valid this pointer before
	// using it.
	GLFWwindow *m_pWindow = nullptr;

	bool        m_bMouseLButtonPressed = false;
	// save the mouse position when mouse is moving, only the mouse left button pressed.
	glm::vec3   m_mousePrePos;
	/* save the mouse position change value relative the m_mousePrePos,
	   only the mouse left button pressed. */
	glm::vec3   m_mouseDeltaPos;

	std::array<double, 2> m_mouseOriginPos;
};

using GraphicSpObject = std::shared_ptr<GraphicBase>;

