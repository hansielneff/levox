#ifndef VOXEL_MESH_H
#define VOXEL_MESH_H

#include "types.h"

#define VOXEL_VERTEX_COUNT 8
#define VOXEL_INDEX_COUNT 6 * 6

struct VoxelData
{
    f32 r, g, b, a;
};

struct VoxelVertex
{
    f32 x, y, z;
    f32 r, g, b;
};

class VoxelMesh
{
public:
    VoxelMesh(VoxelData *voxelData, u32 width, u32 height, u32 depth);
    ~VoxelMesh();

    void render();

private:
    u32 VAO;
    u32 EBO;
    u32 VBO;
    VoxelData *voxelData;
    u32 voxelCount;
    u32 vertexCount;
    u32 indexCount;

    void generateMesh(VoxelVertex *vertices, u32 *indices, u32 width, u32 height);
    void initGLBuffers(const VoxelVertex *vertices, const u32 *indices);
};

#endif
