#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

#include <iostream>

#include "event_handler.hpp"
#include "camera.hpp"
#include "voxel_mesh.hpp"
#include "toolbox.hpp"
#include "shader_source.hpp"
#include "test_model.hpp"

int main()
{
    try
    {
        sf::RenderWindow window({1280, 720}, "Levox",
            sf::Style::Default, sf::ContextSettings(24, 8, 4, 3, 3));
        window.setActive();

        if (!gladLoaderLoadGL())
            throw std::runtime_error("Error: Failed to load OpenGL extension libraries");

        if (!ImGui::SFML::Init(window))
            throw std::runtime_error("Error: Failed to initialize ImGui");

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glEnable(GL_DEPTH_TEST);

        Camera camera(glm::vec3(0.0f, 0.0f, 50.0f));
        Shader shader(vertexShaderSource, fragmentShaderSource);
        VoxelArray *testModel = allocTestModel();
        VoxelMesh voxelMesh(testModel);
        Toolbox toolbox;

        sf::Clock deltaClock;
        sf::Time deltaTime = sf::Time::Zero;
        bool isAppRunning = true;
        while (isAppRunning)
        {
            ImGui::SFML::Update(window, deltaTime);
            handleWindowEvents(window, camera, voxelMesh, toolbox, isAppRunning);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            voxelMesh.render(window, camera, shader);
            window.pushGLStates();
            ImGui::SFML::Render(window);
            window.popGLStates();
            window.display();
            deltaTime = deltaClock.restart();
        }
        window.setActive(false);
        window.close();
        ImGui::SFML::Shutdown();
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
    return 0;
}
