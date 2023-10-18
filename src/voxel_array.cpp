#include <fstream>
#include <cassert>

#include "voxel_array.hpp"

VoxelArray *voxelArrayCreate(u32 width, u32 height, u32 depth)
{
    assert(width > 0 && height > 0 && depth > 0);
    VoxelArray *voxelArray = (VoxelArray*) new char
        [3 * sizeof(u32) + width * height * depth * 4 * sizeof(f32)];
    voxelArray->width = width;
    voxelArray->height = height;
    voxelArray->depth = depth;
    for (u32 i = 0; i < width * height * depth; i++)
        voxelArray->data[i] = {1.0f, 1.0f, 1.0f, 1.0f};
    return voxelArray;
}

void voxelArrayDestroy(VoxelArray *voxelArray)
{
    delete[] voxelArray;
}

VoxelArray *voxelArrayReadFile(const std::string &fileName)
{
    std::ifstream input(fileName, std::ios::binary);
    u32 width, height, depth;
    input.read((char*)&width, sizeof(u32));
    input.read((char*)&height, sizeof(u32));
    input.read((char*)&depth, sizeof(u32));
    VoxelArray *voxelArray = voxelArrayCreate(width, height, depth);
    input.read((char*)&voxelArray->data, width * height * depth * 4 * sizeof(f32));
    return voxelArray;
}

void voxelArrayWriteFile(const std::string &fileName, VoxelArray *voxelArray)
{
    u32 width = voxelArray->width;
    u32 height = voxelArray->height;
    u32 depth = voxelArray->depth;
    std::ofstream output(fileName, std::ios::binary | std::ios::trunc);
    output.write((char*)voxelArray, (3 + width * height * depth * 4) * sizeof(u32));
}
