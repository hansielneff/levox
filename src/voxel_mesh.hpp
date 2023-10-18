#ifndef VOXEL_MESH_HPP
#define VOXEL_MESH_HPP

#include <SFML/Graphics.hpp>

#include "types.hpp"
#include "voxel_array.hpp"
#include "camera.hpp"
#include "shader.hpp"

#define VOXEL_VERTEX_COUNT 8
#define VOXEL_INDEX_COUNT 6 * 6

struct VoxelVertex
{
    f32 x, y, z;
    f32 r, g, b;
};

class VoxelMesh
{
public:
    VoxelArray *voxelArray;

    VoxelMesh(u32 width, u32 height, u32 depth);
    ~VoxelMesh();

    void generateMesh(VoxelArray *voxelArray = NULL);

    void render(const Camera &camera) const;

private:
    u32 vertexCount;
    u32 indexCount;

    u32 VAO;
    u32 EBO;
    u32 VBO;

    Shader shader;

    void bufferMeshData(const VoxelVertex *vertices, const u32 *indices);
};

#endif
