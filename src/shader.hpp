#ifndef SHADER_HPP
#define SHADER_HPP

#include "glad/gl.h"
#include "glad/egl.h"
#ifdef X11_FOUND
    #include "glad/glx.h"
#endif

#include "types.hpp"

class Shader
{
public:
    u32 ID;
    const char *vertexSource;
    const char *fragmentSource;

    Shader(const char* vertexPath, const char* fragmentPath, bool from_files = false);
    Shader(const Shader &other);
    Shader &operator=(const Shader &other);
    ~Shader();

private:
    void loadSourceFromFiles(const char* vertexPath, const char* fragmentPath);
    u32 compileShaderSource(GLenum shaderType, const char *source) const;
    u32 createShaderProgram(u32 vertexShader, u32 fragmentShader, bool deleteShaders) const;
};

#endif
