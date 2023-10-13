#include <SFML/Graphics.hpp>
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
#include "levox_imgui.h"
#include "camera.h"
#include "shader.h"
#include "shader_source.h"
#include "voxel_mesh.h"
#include "test_model.h"

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

static void renderModel(sf::RenderWindow &window, Camera &camera)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Shader shader(vertexShaderSource, fragmentShaderSource);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = camera.getProjectionMatrix();

    glUseProgram(shader.ID);

    u32 modelLoc = glGetUniformLocation(shader.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    u32 viewLoc = glGetUniformLocation(shader.ID, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    u32 projectionLoc = glGetUniformLocation(shader.ID, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    VoxelMesh voxelMesh((VoxelData*)testModel, 4, 4, 4);
    voxelMesh.render();
}

i32 main()
{
    try
    {
        sf::RenderWindow window({1280, 720}, "Levox",
            sf::Style::Default, sf::ContextSettings(24, 8, 4, 3, 3));
        window.setActive();
        initImGui(window);

        if (!gladLoaderLoadGL())
            throw std::exception("Error: Failed to load OpenGL extension libraries");
        glEnable(GL_DEPTH_TEST);

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
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
    return 0;
}
