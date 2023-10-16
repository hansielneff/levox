#include "glad/gl.h"
#include "glad/egl.h"
#ifdef X11_FOUND
    #include "glad/glx.h"
#endif
#include <imgui-SFML.h>

#include <iostream>

#include "event_handler.hpp"
#include "test_model.hpp"

void handleWindowEvents(sf::Window &window, Camera &camera, VoxelMesh &voxelMesh, bool &isAppRunning)
{
    static bool middleMouseDown = false;
    static bool leftShiftDown = false;

    for (auto event = sf::Event(); window.pollEvent(event);)
    {
        ImGui::SFML::ProcessEvent(event);
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
                else
                {
                    sf::Vector2u windowSize = window.getSize();
                    glm::vec3 worldPos = camera.screenToWorld(
                        event.mouseButton.x, event.mouseButton.y, windowSize.x, windowSize.y);
                    i32 gridX = worldPos.x;
                    i32 gridY = worldPos.y;
                    i32 gridZ = worldPos.z;
                    std::cout << "VOXEL: (" << gridX << ", " << gridY << ", " << gridZ << ")" << std::endl;
                    testModel2[gridZ][gridY][gridX].a = 0.0f;
                    voxelMesh.generateMesh((VoxelData*)testModel2, 4, 4, 3);
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
                else
                {
                    sf::Vector2u windowSize = window.getSize();
                    glm::vec3 worldPos = camera.screenToWorld(
                        event.mouseMove.x, event.mouseMove.y, windowSize.x, windowSize.y);
                    std::cout << worldPos.x << ", " << worldPos.y << ", " << worldPos.z << std::endl;
                    i32 gridX = worldPos.x;
                    i32 gridY = worldPos.y;
                    i32 gridZ = worldPos.z;
                    std::cout << gridX << ", " << gridY << ", " << gridZ << std::endl;
                }
            } break;
            default:
                break;
        }
    }
}
