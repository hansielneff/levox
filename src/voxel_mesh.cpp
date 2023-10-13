#include "glad/gl.h"
#include "glad/egl.h"
#ifdef X11_FOUND
    #include "glad/glx.h"
#endif

#include "voxel_mesh.hpp"

VoxelMesh::VoxelMesh(VoxelData *voxelData, u32 width, u32 height, u32 depth)
: voxelData(voxelData)
, voxelCount(width * height * depth)
, vertexCount(voxelCount * VOXEL_VERTEX_COUNT)
, indexCount(voxelCount * VOXEL_INDEX_COUNT)
{
    VoxelVertex *vertices = new VoxelVertex[vertexCount];
    u32 *indices = new u32[indexCount];

    generateMesh(vertices, indices, width, height);
    initGLBuffers(vertices, indices);

    delete[] vertices;
    delete[] indices;
}

VoxelMesh::~VoxelMesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
}

void VoxelMesh::render()
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 2304, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

// TODO: Don't waste time rendering obscured faces
void VoxelMesh::generateMesh(VoxelVertex *vertices, u32 *indices, u32 width, u32 height)
{
    VoxelVertex *vertex = vertices;
    u32 *index = indices;
    u32 invisibleVoxels = 0;
    for (u32 voxelIndex = 0; voxelIndex < voxelCount; voxelIndex++)
    {
        const VoxelData &voxel = voxelData[voxelIndex];
        if (voxel.a < 1.0f)
        {
            invisibleVoxels++;
            continue;
        }

        // These are the local position offsets
        // that make up a cube (8 unique vertices)
        static const f32 vertex_offsets[] = {
            0.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f,
            0.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
        };

        f32 z = voxelIndex / (width * height);
        f32 y = (voxelIndex % (width * height)) / width;
        f32 x = (voxelIndex % (width * height)) % width;
        for (u32 i = 0; i < 8; i++)
        {
            *(vertex++) = {
                x + vertex_offsets[i * 3],
                y + vertex_offsets[i * 3 + 1],
                z + vertex_offsets[i * 3 + 2],
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
}

void VoxelMesh::initGLBuffers(const VoxelVertex *vertices, const u32 *indices)
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(*indices), indices, GL_STATIC_DRAW);

    glGenBuffers(1, &VBO);
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
