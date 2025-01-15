#include "GraphicProgram.h"

#include "Utils/utils.h"

GraphicProgram::GraphicProgram() {

}

GraphicProgram::GraphicProgram(GLuint program) {
    m_vProgram = program;
}

GraphicProgram::GraphicProgram(std::string vertexShaderPath, std::string fragmentShaderPath) {
    m_vProgram = Utils::createShaderProgram(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
}

GraphicProgram::~GraphicProgram() {
    glDeleteProgram(m_vProgram);
}

bool GraphicProgram::init() {
    return true;
}

void GraphicProgram::useProgram() {
    glUseProgram(m_vProgram);
}

GLuint GraphicProgram::getUniformLoc(std::string &uniformName) {
    return glGetUniformLocation(m_vProgram, uniformName.c_str());
}

GLuint GraphicProgram::getUniformLoc(const char* uniforName) {
    return glGetUniformLocation(m_vProgram, uniforName);
}

GLuint GraphicProgram::getProgramID() const {
    return m_vProgram;
}
