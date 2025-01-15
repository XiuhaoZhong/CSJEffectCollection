#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <iomanip>

#include "Graphics/GraphicsHeader.h"

using std::string;

class ArgBase {
public:
    ArgBase() {}
    virtual ~ArgBase() {}
    virtual void Format(std::ostringstream &ss, const std::string &fmt) = 0;
};

template <class T>
class Arg : public ArgBase {
public:
    Arg(T arg) : m_arg(arg) {}
    virtual ~Arg() {}

    virtual void Format(std::ostringstream &ss, const std::string &fmt) {
        ss << m_arg;
    }

private:
    T m_arg;
};

class ArgArray : public std::vector<ArgBase *> {
public:
    ArgArray() {}
    ~ArgArray() {
        std::for_each(begin(), end(), [](ArgBase *p) {
            delete p;
                      });
    }
};

static void FormatItem(std::ostringstream &ss,
                       const std::string &item,
                       const ArgArray &args) {
    int index = 0;
    int alignment = 0;
    std::string fmt;

    char *endptr = nullptr;
    index = strtol(&item[0], &endptr, 10);
    if (index < 0 || index >= args.size()) {
        return;
    }

    if (*endptr == ',') {
        alignment = strtol(endptr + 1, &endptr, 10);
        if (alignment > 0) {
            ss << std::right << std::setw(alignment);
        } else if (alignment < 0) {
            ss << std::left << std::setw(-alignment);
        }
    }

    if (*endptr == ':') {
        fmt = endptr + 1;
    }

    args[index]->Format(ss, fmt);
}

template <class T>
static void Transfer(ArgArray &argArray, T t) {
    argArray.push_back(new Arg<T>(t));
}

template <class T, typename... Args>
static void Transfer(ArgArray &argArray, T t, Args&&... args) {
    Transfer(argArray, t);
    Transfer(argArray, args...);
}

class Utils {
public:
	Utils();
	~Utils();

    // 获取当前运行路径;
    static std::string getCurRunPath();
    static std::string getResourcePath(std::string relativePath);
    static void outputLog(std::string logStr);

	static string readFile(const char* filePath);

    // 以一个图片生成一个纹理;
    static GLuint loadTextureWithFile(const char* textImagePath);

    static float toRadians(float degrees);

    static bool checkOpenGLError();
    static GLuint createShaderProgram(const char *vp, const char *fp);
    static GLuint createShaderProgram(const char *vp, const char *gp, const char *fp);
    static GLuint createShaderProgram(const char *vp, const char *tCS, const char* tES, const char *fp);
    static GLuint createShaderProgram(const char *vp, const char *tCS, const char* tES, char *gp, const char *fp);
    static GLuint loadTexture(const char *texImagePath);
    static GLuint loadCubeMap(const char *mapDir);

    static float* goldAmbient();
    static float* goldDiffuse();
    static float* goldSpecular();
    static float goldShininess();

    static float* silverAmbient();
    static float* silverDiffuse();
    static float* silverSpecular();
    static float silverShininess();

    static float* bronzeAmbient();
    static float* bronzeDiffuse();
    static float* bronzeSpecular();
    static float bronzeShininess();

    static float *globalAmbient();
    static float *lightAmbient();
    static float *lightDiffuse();
    static float *lightSpecular();

    template <typename... Args>
    static string formatString(const std::string &format, Args&&... args);

private:
    static std::string readShaderFile(const char *filePath);
    static void printShaderLog(GLuint shader);
    static void printProgramLog(int prog);
    static GLuint prepareShader(int shaderTYPE, const char *shaderPath);
    static int finalizeShaderProgram(GLuint sprogram);
};

template <typename... Args>
string Utils::formatString(const std::string &format, Args&&... args) {
    if (sizeof...(args) == 0) {
        return format;
    }

    ArgArray argArray;
    Transfer(argArray, args...);
    size_t start = 0;
    size_t pos = 0;
    std::ostringstream ss;

    while (true) {
        pos = format.find('{', start);
        if (pos == std::string::npos) {
            ss << format.substr(start);
            break;
        }

        ss << format.substr(start, pos - start);
        if (format[pos + 1] == '{') {
            ss << '{';
            start = pos + 2;
            continue;
        }

        start = pos + 1;
        pos = format.find('}', start);
        if (pos == std::string::npos) {
            ss << format.substr(start - 1);
            break;
        }

        FormatItem(ss, format.substr(start, pos - start), argArray);
        start = pos + 1;
    }

    return ss.str();
}