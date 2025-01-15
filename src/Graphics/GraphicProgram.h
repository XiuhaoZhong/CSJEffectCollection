#ifndef __GRAPHICSHADER_H__
#define __GRAPHICSHADER_H__

#include <string>
#include <memory>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

class GraphicProgram {
public:
    GraphicProgram();
    GraphicProgram(GLuint program);
    GraphicProgram(std::string vertexShaderPath, std::string fragmentShaderPath);
    ~GraphicProgram();

    bool init();

    void useProgram();

    /**
     * @brief Get the location of a uniform variable.
     */
    GLuint getUniformLoc(std::string& uniformName);

    GLuint getUniformLoc(const char* uniforName);

    GLuint getProgramID() const;

private:
    bool m_bIsInit;

    GLuint m_vProgram;
};

using GraphicSpProgram = std::shared_ptr<GraphicProgram>;

#endif // __GRAPHICSHADER_H__

