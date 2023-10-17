#include <imgui-SFML.h>
#include <imgui.h>

#include "toolbox.hpp"
#include "types.hpp"

void Toolbox::renderImGui()
{
    ImGui::Begin("Toolbox", NULL, ImGuiWindowFlags_MenuBar);
    ImGui::ColorEdit4("Color", color);
    ImGui::RadioButton("Draw", (i32*)&activeTool, (i32)Tool::Draw);
    ImGui::RadioButton("Erase", (i32*)&activeTool, (i32)Tool::Erase);
    ImGui::RadioButton("Paint", (i32*)&activeTool, (i32)Tool::Paint);
    ImGui::RadioButton("Eyedropper", (i32*)&activeTool, (i32)Tool::Eyedropper);
    ImGui::End();
}

void Toolbox::useActiveTool(VoxelArray *voxelArray, glm::ivec3 gridPos, FaceAxis faceAxis, glm::vec3 lookDir)
{
    u32 width = voxelArray->width;
    u32 height = voxelArray->height;
    u32 depth = voxelArray->depth;
    RgbaData &voxel = voxelArray->data[gridPos.z * width * height + gridPos.y * width + gridPos.x];

    switch (activeTool)
    {
        case Tool::Draw:
        {
            if (faceAxis == FaceAxis::X) gridPos.x += lookDir.x > 0.0f ? 1 : -1;
            if (faceAxis == FaceAxis::Y) gridPos.y += lookDir.y > 0.0f ? 1 : -1;
            if (faceAxis == FaceAxis::Z) gridPos.z += lookDir.z > 0.0f ? 1 : -1;
            RgbaData &neighbor_voxel = voxelArray->data[gridPos.z * width * height + gridPos.y * width + gridPos.x];
            neighbor_voxel.r = color[0];
            neighbor_voxel.g = color[1];
            neighbor_voxel.b = color[2];
            neighbor_voxel.a = color[3];
        } break;
        case Tool::Erase:
        {
            voxel.a = 0.0f;
        } break;
        case Tool::Paint:
        {
            voxel.r = color[0];
            voxel.g = color[1];
            voxel.b = color[2];
            voxel.a = color[3];
        } break;
        case Tool::Eyedropper:
        {
            color[0] = voxel.r;
            color[1] = voxel.g;
            color[2] = voxel.b;
            color[3] = voxel.a;
        } break;
    }
}