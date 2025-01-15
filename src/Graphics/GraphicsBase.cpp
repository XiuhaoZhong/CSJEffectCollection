#include "GraphicsBase.h"

GraphicBase::GraphicBase() {
	m_mousePrePos = glm::vec3(0.0, 0.0, 0.0);
}

GraphicBase::~GraphicBase() {

}

bool GraphicBase::createShader(string& vertexPath, string& fragmentPath) {

	bool createVertShaderRes = false, createFragShaderRes = false;
	if (vertexPath.size() == 0) {
		createVertShaderRes = false;
	}

	if (fragmentPath.size() == 0) {
		createFragShaderRes = false;
	}

	GLint success;
	GLchar infoLog[512];

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	string src = Utils::readFile(vertexPath.c_str());
	const char* vertShaderSrc = src.c_str();
	//const char* vertShaderSrc = Utils::readFile(vertexPath->c_str()).c_str();
	glShaderSource(vertexShader, 1, &vertShaderSrc, NULL);
	glCompileShader(vertexShader);
	// get the errors;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << std::endl;
	} else {
		createVertShaderRes = true;
		this->m_vVertexShader = vertexShader;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	string fragSrc = Utils::readFile(fragmentPath.c_str());
	const char *fragShaderSrc = fragSrc.c_str();
	//const char* fragShaderSrc = Utils::readFile(fragmentPath->c_str()).c_str();
	glShaderSource(fragmentShader, 1, &fragShaderSrc, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << std::endl;
	} else {
		createFragShaderRes = true;
		this->m_vFragmentShader = fragmentShader;
	}

	GLuint vfprogram = glCreateProgram();
	glAttachShader(vfprogram, this->m_vVertexShader);
	glAttachShader(vfprogram, this->m_vFragmentShader);

	glLinkProgram(vfprogram);
	this->m_vRenderingProgram = vfprogram;

	return (createVertShaderRes && createFragShaderRes);
}

void GraphicBase::useShaders() {
	glUseProgram(this->m_vRenderingProgram);
}

void GraphicBase::useProgram(GraphicSpProgram program) {
	if (!program) {
		return;
	}

	program->useProgram();
}

void GraphicBase::mouseMove(double xCoord, double yCoord) {
	std::string log = Utils::formatString("Mouse move event, current position: ({0}, {1})", xCoord, yCoord);
	//Utils::outputLog(log);

	if (m_bMouseLButtonPressed) {
		// Get the current pos in world coordinates.
		glm::vec3 curPos = translateMousePos(xCoord, yCoord);
		// computer the delta pos.
		m_mouseDeltaPos = glm::vec3(curPos - m_mousePrePos);
		// record the newest position of the mouse.
		m_mousePrePos = glm::vec3(curPos);
	}

	onMouseMove(xCoord, yCoord);
}

void GraphicBase::mouseScroll(double xOffset, double yOffset) {
    std::string log = Utils::formatString("Mouse scroll event, current offset: ({0}, {1})", xOffset, yOffset);
	Utils::outputLog(log);

	onMouseScroll(xOffset, yOffset);
}

void GraphicBase::framebufferSizeChanged(int width, int height) {
    std::string log = Utils::formatString("Framebuffer change event, current size: {0}x{1}", width, height);
    //Utils::outputLog(log);

	onResize(width, height);
}

void GraphicBase::mouseButtonClicked(int mouseBtn, int event, int modes) {
	switch (mouseBtn) {
	case GLFW_MOUSE_BUTTON_LEFT:
		if (event == GLFW_PRESS) {
            //Utils::outputLog("Mouse LButton pressed!");
			m_bMouseLButtonPressed = true;

            double xCoord, yCoord;
            glfwGetCursorPos(m_pWindow, &xCoord, &yCoord);
			m_mouseOriginPos[0] = xCoord;
			m_mouseOriginPos[1] = yCoord;

            glm::vec3 glCoord = translateMousePos(xCoord, yCoord);
			m_mousePrePos = glm::vec3(glCoord);

            std::string log = Utils::formatString("Mouse LButton pressed, coordinates: ({0}, {1}, {2})", glCoord.x, glCoord.y, glCoord.z);
            Utils::outputLog(log);

			onMouseLButtonPressed();
		} else if (event == GLFW_RELEASE) {
            //Utils::outputLog("Mouse LButton released!");
			m_bMouseLButtonPressed = false;
			m_mousePrePos = glm::vec3(0.0, 0.0, 0.0);
			m_mouseDeltaPos = glm::vec3(0.0, 0.0, 0.0);

			onMouseLButtonReleased();
		} else {
			Utils::outputLog("other mouse click event");
		}
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
        if (event == GLFW_PRESS) {
            //Utils::outputLog("Mouse RButton pressed!");
			onMouseRButtonPressed();
        } else if (event == GLFW_RELEASE) {
            //Utils::outputLog("Mouse RButton released!");
			onMouseRButtonReleased();
        } else {
			Utils::outputLog("other mouse click event");
        }
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
        if (event == GLFW_PRESS) {
            //Utils::outputLog("Mouse MButton pressed!");
			onMouseMButtonPressed();
        } else if (event == GLFW_RELEASE) {
            //Utils::outputLog("Mouse MButton released!");
			onMouseMButtonReleased();
        } else {
			Utils::outputLog("other mouse click event");
        }
		break;
	default:
		break;
	}
}

glm::vec3 GraphicBase::translateMousePos(double xCoord, double yCoord) {
	GLdouble modelView[16];
	GLdouble projView[16];
	GLint viewPort[4];
	GLdouble worldX, worldY, worldZ;

	glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
	glGetDoublev(GL_PROJECTION_MATRIX, projView);
	glGetIntegerv(GL_VIEWPORT, viewPort);

	gluUnProject(xCoord, yCoord, 0, modelView, projView, viewPort, &worldX, &worldY, &worldZ);
	glm::vec3 glCoord = glm::vec3(worldX, worldY, worldZ);

	if (m_bMouseLButtonPressed) {
		
	}

	return glCoord;
}

void GraphicBase::onMouseLButtonPressed() {
	
}

void GraphicBase::onMouseLButtonReleased() {
	
}

void GraphicBase::onMouseRButtonPressed() {
    
}

void GraphicBase::onMouseRButtonReleased() {
    
}

void GraphicBase::onMouseMButtonPressed() {
    
}

void GraphicBase::onMouseMButtonReleased() {
    
}

void GraphicBase::onMouseMove(double xCoord, double yCoord) {

}

void GraphicBase::onMouseScroll(double xOffset, double yOffset) {

}

void GraphicBase::onResize(int width, int height) {
	
}

