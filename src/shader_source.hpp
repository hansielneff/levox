#ifndef SHADER_SOURCE_HPP
#define SHADER_SOURCE_HPP

static const char *voxelMeshVertexShader =
R"(
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;

out vec3 vertexColor;
out vec3 vertexPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    vertexColor = aCol;
    vertexPosition = aPos;
}
)";

static const char *voxelMeshFragmentShader =
R"(
#version 330 core

in vec3 vertexColor;
in vec3 vertexPosition;

out vec4 fragColor;

uniform bool showGrid;
uniform float gridThickness;
uniform vec3 gridColor;

void main()
{
    fragColor = vec4(vertexColor, 1.0f);

    if (showGrid)
    {
        vec3 fracts = vec3(fract(vertexPosition.x), fract(vertexPosition.y), fract(vertexPosition.z));
        bool inGridRangeX = (fracts.x < gridThickness && fract(vertexPosition.x) > 0.00001f) || (fracts.x > 1.0f - gridThickness && fract(vertexPosition.x) < 0.9999f);
        bool inGridRangeY = (fracts.y < gridThickness && fract(vertexPosition.y) > 0.00001f) || (fracts.y > 1.0f - gridThickness && fract(vertexPosition.y) < 0.9999f);
        bool inGridRangeZ = (fracts.z < gridThickness && fract(vertexPosition.z) > 0.00001f) || (fracts.z > 1.0f - gridThickness && fract(vertexPosition.z) < 0.9999f);

        if (inGridRangeX || inGridRangeY || inGridRangeZ)
            fragColor = vec4(gridColor, 1.0f);
    }
}
)";

#endif
