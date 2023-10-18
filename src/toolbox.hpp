#ifndef TOOLBOX_HPP
#define TOOLBOX_HPP

#include "voxel_mesh.hpp"

enum class FaceAxis { X, Y, Z };

enum class Tool
{
    Draw,
    Erase,
    Paint,
    Eyedropper
};

class Toolbox
{
public:
    void renderImGui(VoxelMesh &voxelMesh);
    void useActiveTool(VoxelArray &voxelArray, glm::ivec3 gridPos, FaceAxis faceAxis, glm::vec3 lookDir);

private:
    Tool activeTool = Tool::Draw;
    f32 color[4] = {1.0f, 0.0f, 0.0f, 1.0f};
};

#endif
