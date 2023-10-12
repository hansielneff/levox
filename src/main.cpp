#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glad/gl.h"
#include "glad/egl.h"
#ifdef X11_FOUND
    #include "glad/glx.h"
#endif

#include <iostream>

#include "types.h"
#include "camera.h"
#include "shader.h"
#include "shader_source.h"
#include "voxel.h"

static void printContextSettings(sf::Window &window)
{
    auto settings = window.getSettings();
    std::cout << "depth bits:" << settings.depthBits << std::endl;
    std::cout << "stencil bits:" << settings.stencilBits << std::endl;
    std::cout << "antialiasing level:" << settings.antialiasingLevel << std::endl;
    std::cout << "version:" << settings.majorVersion << "." << settings.minorVersion << std::endl;
}

static void handleWindowEvents(sf::Window &window, Camera &camera, bool &isAppRunning)
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

static void renderImGui(sf::RenderWindow &window, sf::Time delta)
{
    // TODO: These push/pop operations are rather expensive to do every render cycle,
    // so find/create an OpenGL backend for ImGui instead of the imgui-sfml library.
    // Once this is done, change the SFML window back to a regular sf::Window.

    // ImGui relies on SFML's graphics module, which might change OpenGL state,
    // so we push and pop our state to preserve it.
    window.pushGLStates();
    ImGui::SFML::Update(window, delta);
    ImGui::ShowDemoWindow();
    ImGui::SFML::Render(window);
    window.popGLStates();
}

static void renderModel(sf::RenderWindow &window, Camera &camera)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Shader shader(vertexShaderSource, fragmentShaderSource);
    VoxelMesh voxelMesh;

    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = camera.getProjectionMatrix();

    glUseProgram(shader.ID);

    // TODO: This is a *very* naive rendering method as every
    // voxel is its own separate draw call and obscured faces
    // are rendered as well. Please fix.
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            for (int z = 0; z < 4; z++) {
                VoxelData voxel = testModel[x][y][z];
                if (voxel.a < 1.0f) continue;

                glm::mat4 model = glm::mat4(1.0f);
                model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
                model = glm::translate(model, glm::vec3(x, y, z));

                u32 modelLoc = glGetUniformLocation(shader.ID, "model");
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

                u32 viewLoc = glGetUniformLocation(shader.ID, "view");
                glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

                u32 projectionLoc = glGetUniformLocation(shader.ID, "projection");
                glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
                
                u32 colorLoc = glGetUniformLocation(shader.ID, "color");
                glUniform4f(colorLoc, voxel.r, voxel.g, voxel.b, voxel.a);

                voxelMesh.bind();
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);
            }
        }
    }
}

i32 main()
{
    sf::RenderWindow window({1280, 720}, "Levox",
        sf::Style::Default, sf::ContextSettings(24, 8, 4, 3, 3));
    window.setActive();
    printContextSettings(window);

    if (!gladLoaderLoadGL())
    {
        std::cerr << "Error: Failed to load OpenGL extension libraries" << std::endl;
        return 1;
    }

    glEnable(GL_DEPTH_TEST);

    if (!ImGui::SFML::Init(window))
    {
        std::cerr << "Error: Failed to initialize ImGui" << std::endl;
        return 1;
    }

    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
    sf::Clock deltaClock;
    sf::Time deltaTime = sf::Time::Zero;
    bool isAppRunning = true;
    while (isAppRunning)
    {
        handleWindowEvents(window, camera, isAppRunning);
        renderModel(window, camera);
        renderImGui(window, deltaTime);
        window.display();
        deltaTime = deltaClock.restart();
    }

    window.setActive(false);
    ImGui::SFML::Shutdown();
    return 0;
}
