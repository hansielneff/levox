#ifndef VOXEL_ARRAY_HPP
#define VOXEL_ARRAY_HPP

#include <string>

#include "types.hpp"

struct RgbaData
{
    f32 r, g, b, a;
};

struct VoxelArray
{
    u32 width, height, depth;
    RgbaData data[];
};

VoxelArray *voxelArrayCreate(u32 width, u32 height, u32 depth);
void voxelArrayDestroy(VoxelArray *voxelArray);

VoxelArray *voxelArrayReadFile(const std::string &fileName);
void voxelArrayWriteFile(const std::string &fileName, VoxelArray *voxelArray);

#endif
