#include <imgui-SFML.h>
#include <imgui.h>
#include "nfd.h"

#include "toolbox.hpp"
#include "types.hpp"

void Toolbox::renderImGui(VoxelMesh &voxelMesh)
{
    static glm::ivec3 modelSize = {16, 16, 16};

    ImGui::SetNextWindowSize(ImVec2(250, 190));
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("Toolbox", NULL, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize);
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New", "Ctrl+N"))
            {
                voxelMesh.generateMesh(voxelArrayCreate(
                    modelSize.x, modelSize.y, modelSize.z));
            }
            if (ImGui::MenuItem("Open..", "Ctrl+O"))
            {
                nfdchar_t *outPath = NULL;
                nfdresult_t result = NFD_OpenDialog("levox", NULL, &outPath);

                if (result == NFD_OKAY)
                    voxelMesh.generateMesh(voxelArrayReadFile(outPath));
                else if (result != NFD_CANCEL)
                    throw std::runtime_error("Error: failed to open file");
            }
            if (ImGui::MenuItem("Save", "Ctrl+S"))
            {
                nfdchar_t *outPath = NULL;
                nfdresult_t result = NFD_SaveDialog("levox", NULL, &outPath);

                if (result == NFD_OKAY)
                    voxelArrayWriteFile(outPath, voxelMesh.voxelArray);
                else if (result != NFD_CANCEL)
                    throw std::runtime_error("Error: failed to save file");
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::DragInt3("Size", (i32*)&modelSize, 0.1f, 1, 64);
    ImGui::ColorEdit4("Color", color);
    ImGui::RadioButton("Draw", (i32*)&activeTool, (i32)Tool::Draw);
    ImGui::RadioButton("Erase", (i32*)&activeTool, (i32)Tool::Erase);
    ImGui::RadioButton("Paint", (i32*)&activeTool, (i32)Tool::Paint);
    ImGui::RadioButton("Eyedropper", (i32*)&activeTool, (i32)Tool::Eyedropper);
    ImGui::End();
}

void Toolbox::useActiveTool(VoxelArray &voxelArray, glm::ivec3 gridPos, FaceAxis faceAxis, glm::vec3 lookDir)
{
    u32 width = voxelArray.width;
    u32 height = voxelArray.height;
    u32 depth = voxelArray.depth;
    
    if (gridPos.x >= width || gridPos.y >= height || gridPos.z >= depth)
        return;
    
    RgbaData &voxel = voxelArray.data[gridPos.z * width * height + gridPos.y * width + gridPos.x];

    if (voxel.a != 1.0f) return;

    switch (activeTool)
    {
        case Tool::Draw:
        {
            if (faceAxis == FaceAxis::X) gridPos.x += lookDir.x > 0.0f ? 1 : -1;
            if (faceAxis == FaceAxis::Y) gridPos.y += lookDir.y > 0.0f ? 1 : -1;
            if (faceAxis == FaceAxis::Z) gridPos.z += lookDir.z > 0.0f ? 1 : -1;

            if (gridPos.x >= width || gridPos.y >= height || gridPos.z >= depth)
                return;

            RgbaData &neighbor_voxel = voxelArray.data[gridPos.z * width * height + gridPos.y * width + gridPos.x];
            if (neighbor_voxel.a == 1.0f) return;
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