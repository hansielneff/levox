#include <SFML/Graphics.hpp>

#include <iostream>

#include "event_handler.h"
#include "levox_imgui.h"
#include "graphics.h"
#include "camera.h"

int main()
{
    try
    {
        sf::RenderWindow window({1280, 720}, "Levox",
            sf::Style::Default, sf::ContextSettings(24, 8, 4, 3, 3));
        window.setActive();
        initImGui(window);
        initOpenGLContext();

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
