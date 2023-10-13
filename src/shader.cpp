#include "glad/gl.h"
#include "glad/egl.h"
#ifdef X11_FOUND
    #include "glad/glx.h"
#endif

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "shader.h"

Shader::Shader(const char* vertex, const char* fragment, bool from_files)
{
    if (from_files)
    { 
        loadSourceFromFiles(vertex, fragment);
    }
    else
    {
        vertexSource = vertex;
        fragmentSource = fragment;
    }

    u32 vertexShader = compileShaderSource(GL_VERTEX_SHADER, vertexSource);
    u32 fragmentShader = compileShaderSource(GL_FRAGMENT_SHADER, fragmentSource);
    ID = createShaderProgram(vertexShader, fragmentShader, true);
}

Shader::Shader(const Shader &other)
{
    vertexSource = other.vertexSource;
    fragmentSource = other.fragmentSource;
    u32 vertexShader = compileShaderSource(GL_VERTEX_SHADER, vertexSource);
    u32 fragmentShader = compileShaderSource(GL_FRAGMENT_SHADER, fragmentSource);
    ID = createShaderProgram(vertexShader, fragmentShader, true);
}

Shader::~Shader()
{
    glDeleteProgram(ID);
}

Shader &Shader::operator=(const Shader &other)
{
    if (this == &other) return *this;

    glDeleteProgram(ID);

    vertexSource = other.vertexSource;
    fragmentSource = other.fragmentSource;
    u32 vertexShader = compileShaderSource(GL_VERTEX_SHADER, vertexSource);
    u32 fragmentShader = compileShaderSource(GL_FRAGMENT_SHADER, fragmentSource);
    ID = createShaderProgram(vertexShader, fragmentShader, true);
    return *this;
}

void Shader::loadSourceFromFiles(const char* vertexPath, const char* fragmentPath)
{
    try
    {
        std::ifstream vertexFile(vertexPath);
        std::ifstream fragmentFile(fragmentPath);
        std::stringstream vertexStream;
        std::stringstream fragmentStream;

        vertexStream << vertexFile.rdbuf();
        fragmentStream << vertexFile.rdbuf();
        vertexSource = vertexStream.str().c_str();
        fragmentSource = fragmentStream.str().c_str();
    }
    catch(const std::exception &e)
    {
        std::cerr << "Error: Failed to read shader source files\n" << e.what() << std::endl;
        return;
    }
}

u32 Shader::compileShaderSource(GLenum shaderType, const char *source) const
{
    u32 shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    i32 success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        u32 const logSize = 512;
        char infoLog[logSize];
        glGetShaderInfoLog(shader, logSize, NULL, infoLog);
        std::cout << "Error: Failed to compile shader source\n" << infoLog << std::endl;
        return 0;
    }

    return shader;
}

u32 Shader::createShaderProgram(u32 vertexShader, u32 fragmentShader, bool deleteShaders) const
{
    u32 shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    i32 success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        u32 const logSize = 512;
        char infoLog[logSize];
        glGetProgramInfoLog(shaderProgram, logSize, NULL, infoLog);
        std::cout << "Error: Failed to link shader program\n" << infoLog << std::endl;
        return 0;
    }

    if (deleteShaders)
    {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    return shaderProgram;
}
