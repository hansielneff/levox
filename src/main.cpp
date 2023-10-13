#include <SFML/Graphics.hpp>

#include <iostream>

#include "event_handler.hpp"
#include "levox_imgui.hpp"
#include "camera.hpp"
#include "shader_source.hpp"
#include "test_model.hpp"

int main()
{
    try
    {
        sf::RenderWindow window({1280, 720}, "Levox",
            sf::Style::Default, sf::ContextSettings(24, 8, 4, 3, 3));
        window.setActive();
        initImGui(window);

        if (!gladLoaderLoadGL())
            throw std::runtime_error("Error: Failed to load OpenGL extension libraries");
        glEnable(GL_DEPTH_TEST);

        Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
        Shader shader(vertexShaderSource, fragmentShaderSource);
        VoxelMesh voxelMesh((VoxelData*)testModel, 4, 4, 4);
        voxelMesh.generateMesh((VoxelData*)testModel2, 4, 4, 4);

        sf::Clock deltaClock;
        sf::Time deltaTime = sf::Time::Zero;
        bool isAppRunning = true;
        while (isAppRunning)
        {
            handleWindowEvents(window, camera, isAppRunning);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            voxelMesh.render(window, camera, shader);
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
