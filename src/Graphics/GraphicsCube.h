#pragma once

#include "GraphicsBase.h"

class GraphicCube : public GraphicsBase {
public:
	GraphicCube();
	~GraphicCube();

	bool init(GLFWwindow *window) override;
	void display(GLFWwindow *window, double currentTime) override;
private:
	// ��ͼ�����λ�ã����������λ�ã�Ҳ�������ۿ��ķ���
	float m_vCameraX;
	float m_vCameraY;
	float m_vCameraZ;

	// �������λ��;
	float m_vCubeLocX;
	float m_vCubeLocY;
	float m_vCubeLocZ;

	// �������������λ��;
	float m_vPyrLocX;
	float m_vPyrLocY;
	float m_vPyrLocZ;

	GLuint m_vVao[numVAOs];
	GLuint m_vVbo[numVBOs];

};

