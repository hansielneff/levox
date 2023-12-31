#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glad/gl.h"
#include "glad/egl.h"
#ifdef X11_FOUND
    #include "glad/glx.h"
#endif
#include <imgui-SFML.h>
#include <imgui.h>

#include "voxel_mesh.hpp"
#include "shader_source.hpp"

VoxelMesh::VoxelMesh(u32 width, u32 height, u32 depth)
: shader(voxelMeshVertexShader, voxelMeshFragmentShader)
, voxelArray(voxelArrayCreate(width, height, depth))
, vertexCount(0)
, indexCount(0)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO);
    generateMesh();
}

VoxelMesh::~VoxelMesh()
{
    voxelArrayDestroy(voxelArray);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
}

void VoxelMesh::render(const Camera &camera) const
{
    if (indexCount == 0) return;

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = camera.getProjectionMatrix();

    glUseProgram(shader.ID);

    u32 modelLoc = glGetUniformLocation(shader.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    u32 viewLoc = glGetUniformLocation(shader.ID, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    u32 projectionLoc = glGetUniformLocation(shader.ID, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    static bool showGrid = true;
    u32 showGridLoc = glGetUniformLocation(shader.ID, "showGrid");
    glUniform1i(showGridLoc, showGrid);

    static float gridThickness = 0.05f;
    u32 gridThicknessLoc = glGetUniformLocation(shader.ID, "gridThickness");
    glUniform1f(gridThicknessLoc, gridThickness);

    static f32 gridColor[] = {1.0f, 1.0f, 1.0f};
    u32 gridColorLoc = glGetUniformLocation(shader.ID, "gridColor");
    glUniform3fv(gridColorLoc, 1, gridColor);

    ImGui::Begin("Toolbox");
    ImGui::Checkbox("Show grid", &showGrid);
    ImGui::DragFloat("Grid thickness", &gridThickness, 0.001f, 0.0f, 0.25f);
    ImGui::ColorEdit3("Grid color", gridColor);
    ImGui::End();

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

// TODO: Don't waste time rendering obscured faces
void VoxelMesh::generateMesh(VoxelArray *voxelArray)
{
    if (voxelArray)
    {
        if (this->voxelArray && this->voxelArray != voxelArray)
            voxelArrayDestroy(this->voxelArray);
        this->voxelArray = voxelArray;
    }
    else
    {
        voxelArray = this->voxelArray;
        assert(voxelArray != NULL);
    }

    u32 width = voxelArray->width;
    u32 height = voxelArray->height;
    u32 depth = voxelArray->depth;
    u32 voxelCount = width * height * depth;
    vertexCount = voxelCount * VOXEL_VERTEX_COUNT;
    indexCount = voxelCount * VOXEL_INDEX_COUNT;
    VoxelVertex *vertices = new VoxelVertex[vertexCount];
    u32 *indices = new u32[indexCount];

    VoxelVertex *vertex = vertices;
    u32 *index = indices;
    u32 invisibleVoxels = 0;
    for (u32 voxelIndex = 0; voxelIndex < voxelCount; voxelIndex++)
    {
        const RgbaData &voxel = voxelArray->data[voxelIndex];
        if (voxel.a < 1.0f)
        {
            vertexCount -= VOXEL_VERTEX_COUNT;
            indexCount -= VOXEL_INDEX_COUNT;
            invisibleVoxels++;
            continue;
        }

        // These are the local position offsets
        // that make up a cube (8 unique vertices)
        static const glm::vec3 vertex_offsets[] = {
            {0.0f, 0.0f, 0.0f},
            {1.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {1.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 1.0f},
            {1.0f, 0.0f, 1.0f},
            {0.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 1.0f}
        };

        f32 z = voxelIndex / (width * height);
        f32 y = (voxelIndex % (width * height)) / width;
        f32 x = (voxelIndex % (width * height)) % width;
        for (u32 i = 0; i < VOXEL_VERTEX_COUNT; i++)
        {
            *(vertex++) = {
                x + vertex_offsets[i].x,
                y + vertex_offsets[i].y,
                z + vertex_offsets[i].z,
                voxel.r, voxel.g, voxel.b
            };
        }

        // Each line in this array holds the 6 indices
        // needed to draw a specific face
        static const u32 index_offsets[] = {
            0, 1, 2, 3, 1, 2,   // Front
            4, 5, 6, 7, 5, 6,   // Back
            2, 3, 6, 7, 3, 6,   // Top
            0, 1, 4, 5, 1, 4,   // Bottom
            1, 3, 5, 7, 3, 5,   // Right
            0, 2, 4, 6, 2, 4    // Left
        };

        for (u32 i = 0; i < VOXEL_INDEX_COUNT; i++)
            *(index++) = (voxelIndex - invisibleVoxels) *
                VOXEL_VERTEX_COUNT + index_offsets[i];
    }

    bufferMeshData(vertices, indices);

    delete[] vertices;
    delete[] indices;
}

void VoxelMesh::bufferMeshData(const VoxelVertex *vertices, const u32 *indices)
{
    glBindVertexArray(VAO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(*indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(*vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), (void*)(3 * sizeof(f32)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
