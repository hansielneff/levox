#ifndef SHADER_SOURCE_HPP
#define SHADER_SOURCE_HPP

static const char *voxelMeshVertexShader =
R"(
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;

out vec3 vertexColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    vertexColor = aCol;
}
)";

static const char *voxelMeshFragmentShader =
R"(
#version 330 core

in vec3 vertexColor;

out vec4 fragColor;

void main()
{
    fragColor = vec4(vertexColor, 1.0f);
} 
)";

#endif
