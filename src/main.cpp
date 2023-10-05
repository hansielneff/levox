#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

#include <iostream>
#include <thread>

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

static void renderingLoop(sf::RenderWindow *window, const bool *isAppRunning)
{
    ImGui::SFML::Init(*window);
    window->setActive();
    sf::Clock deltaClock;
    while (*isAppRunning)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        window->pushGLStates();
        ImGui::SFML::Update(*window, deltaClock.restart());
        ImGui::ShowDemoWindow();
        ImGui::SFML::Render(*window);
        window->popGLStates();

        window->display();
    }
    window->setActive(false);
    ImGui::SFML::Shutdown();
}

int main()
{
    sf::RenderWindow window({1280, 720}, "Levox", sf::Style::Default, sf::ContextSettings(24, 8, 4));
    printContextSettings(window);

    bool isAppRunning = true;

    window.setActive(false);
    std::thread renderThread(renderingLoop, &window, &isAppRunning);

    while (isAppRunning)
    {
        handleWindowEvents(window, isAppRunning);
    }

    renderThread.join();
    return 0;
}
