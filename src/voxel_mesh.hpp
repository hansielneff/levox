#ifndef VOXEL_MESH_H
#define VOXEL_MESH_H

#include <SFML/Graphics.hpp>

#include "types.hpp"
#include "camera.hpp"
#include "shader.hpp"

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
    VoxelMesh(const VoxelData *voxelData, u32 width, u32 height, u32 depth);
    ~VoxelMesh();

    void generateMesh(const VoxelData *voxelData,
        u32 width, u32 height, u32 depth);

    void render(sf::RenderWindow &window,
        const Camera &camera, const Shader &shader) const;

private:
    u32 VAO;
    u32 EBO;
    u32 VBO;

    void bufferMeshData(const VoxelVertex *vertices, const u32 *indices,
        u32 vertexCount, u32 indexCount);
};

#endif
