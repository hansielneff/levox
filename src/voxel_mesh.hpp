#ifndef VOXEL_MESH_HPP
#define VOXEL_MESH_HPP

#include <SFML/Graphics.hpp>

#include "types.hpp"
#include "camera.hpp"
#include "shader.hpp"

#define VOXEL_VERTEX_COUNT 8
#define VOXEL_INDEX_COUNT 6 * 6

struct RgbaData
{
    f32 r, g, b, a;
};

struct VoxelArray
{
    u32 width, height, depth;
    RgbaData data[];
};

struct VoxelVertex
{
    f32 x, y, z;
    f32 r, g, b;
};

class VoxelMesh
{
public:
    VoxelMesh(const VoxelArray *voxelArray);
    ~VoxelMesh();

    void generateMesh(const VoxelArray *voxelArray);

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
