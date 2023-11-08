#include "glad/gl.h"
#include "glad/egl.h"
#ifdef X11_FOUND
    #include "glad/glx.h"
#endif
#include <imgui-SFML.h>
#include <imgui.h>

#include "event_handler.hpp"
#include "toolbox.hpp"

void handleWindowEvents(sf::Window &window, Camera &camera,
    VoxelMesh &voxelMesh, bool &isAppRunning)
{
    static bool middleMouseDown = false;
    static bool leftShiftDown = false;
    static Toolbox toolbox;
    
    toolbox.renderImGui(voxelMesh);
    ImGuiIO &imGuiIO = ImGui::GetIO();

    for (auto event = sf::Event(); window.pollEvent(event);)
    {

        ImGui::SFML::ProcessEvent(window, event);
        if (imGuiIO.WantCaptureMouse || imGuiIO.WantCaptureKeyboard) continue;

        switch (event.type)
        {
            case sf::Event::Closed:
            {
                isAppRunning = false;
            } break;
            case sf::Event::Resized:
            {
                glViewport(0, 0, event.size.width, event.size.height);
            } break;
            case sf::Event::KeyPressed:
            {
                if (event.key.code == sf::Keyboard::LShift)
                    leftShiftDown = true;
                else if (event.key.code == sf::Keyboard::P)
                    camera.perspective = !camera.perspective;
            } break;
            case sf::Event::KeyReleased:
            {
                if (event.key.code == sf::Keyboard::LShift)
                    leftShiftDown = false;
            } break;
            case sf::Event::MouseWheelScrolled:
            {
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
                    camera.zoom(event.mouseWheelScroll.delta);
            } break;
            case sf::Event::MouseButtonPressed:
            {
                if (event.mouseButton.button == sf::Mouse::Middle)
                    middleMouseDown = true;
                else if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2u windowSize = window.getSize();
                    glm::vec3 worldPos = camera.screenToWorld(
                        event.mouseButton.x, event.mouseButton.y, windowSize.x, windowSize.y);
                    if (glm::length(worldPos) == 0) break;

                    f32 xDistFromWholeNum = glm::abs(worldPos.x - glm::round(worldPos.x));
                    f32 yDistFromWholeNum = glm::abs(worldPos.y - glm::round(worldPos.y));
                    f32 zDistFromWholeNum = glm::abs(worldPos.z - glm::round(worldPos.z));

                    FaceAxis faceAxis;
                    if (xDistFromWholeNum < yDistFromWholeNum && xDistFromWholeNum < zDistFromWholeNum)
                        faceAxis = FaceAxis::X;
                    else if (yDistFromWholeNum < xDistFromWholeNum && yDistFromWholeNum < zDistFromWholeNum)
                        faceAxis = FaceAxis::Y;
                    else if (zDistFromWholeNum < xDistFromWholeNum && zDistFromWholeNum < yDistFromWholeNum)
                        faceAxis = FaceAxis::Z;
                    else
                        throw std::runtime_error("Error: invalid FaceAxis");

                    glm::vec3 lookDir = camera.getLookDir();
                    i32 gridX = faceAxis == FaceAxis::X ? glm::round(worldPos.x) - 1 * (lookDir.x > 0.0f) : worldPos.x;
                    i32 gridY = faceAxis == FaceAxis::Y ? glm::round(worldPos.y) - 1 * (lookDir.y > 0.0f) : worldPos.y;
                    i32 gridZ = faceAxis == FaceAxis::Z ? glm::round(worldPos.z) - 1 * (lookDir.z > 0.0f) : worldPos.z;

                    toolbox.useActiveTool(*voxelMesh.voxelArray, {gridX, gridY, gridZ}, faceAxis, lookDir);
                    voxelMesh.generateMesh(voxelMesh.voxelArray);
                }
            } break;
            case sf::Event::MouseButtonReleased:
            {
                if (event.mouseButton.button == sf::Mouse::Middle)
                    middleMouseDown = false;
            } break;
            case sf::Event::MouseMoved:
            {
                sf::Vector2i currMousePos = {event.mouseMove.x, event.mouseMove.y};
                static sf::Vector2i prevMousePos = currMousePos;
                sf::Vector2i mouseDelta = currMousePos - prevMousePos;
                prevMousePos = currMousePos;
                if (middleMouseDown)
                {
                    leftShiftDown ?
                        camera.pan(mouseDelta.x, mouseDelta.y) :
                        camera.orbit(mouseDelta.x, mouseDelta.y);
                }
            } break;
            default:
                break;
        }
    }
}
