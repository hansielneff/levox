#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

#include <iostream>

static void printContextSettings(sf::Window &window)
{
    auto settings = window.getSettings();
    std::cout << "depth bits:" << settings.depthBits << std::endl;
    std::cout << "stencil bits:" << settings.stencilBits << std::endl;
    std::cout << "antialiasing level:" << settings.antialiasingLevel << std::endl;
    std::cout << "version:" << settings.majorVersion << "." << settings.minorVersion << std::endl;
}

static void handleWindowEvents(sf::Window &window, bool &isAppRunning)
{
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
        }
    }
}

static void render(sf::RenderWindow &window, sf::Time &delta)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    window.pushGLStates();
    ImGui::SFML::Update(window, delta);
    ImGui::ShowDemoWindow();
    ImGui::SFML::Render(window);
    window.popGLStates();

    window.display();
}

int main()
{
    sf::RenderWindow window({1280, 720}, "Levox",
        sf::Style::Default, sf::ContextSettings(24, 8, 4, 3, 3));
    ImGui::SFML::Init(window);
    window.setActive();
    printContextSettings(window);

    sf::Clock deltaClock;
    bool isAppRunning = true;
    while (isAppRunning)
    {
        handleWindowEvents(window, isAppRunning);
        render(window, deltaClock.restart());
    }

    window.setActive(false);
    ImGui::SFML::Shutdown();
    return 0;
}
