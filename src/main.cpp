#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

#include <iostream>

#include "glad/gl.h"
#include "types.h"

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
                isAppRunning = false;
                break;
            case sf::Event::Resized:
                glViewport(0, 0, event.size.width, event.size.height);
                break;
            default:
                break;
        }
    }
}

static void render_imgui(sf::RenderWindow &window, sf::Time delta)
{
    window.pushGLStates();
    ImGui::SFML::Update(window, delta);
    ImGui::ShowDemoWindow();
    ImGui::SFML::Render(window);
    window.popGLStates();
}

static void render_scene(sf::RenderWindow &window)
{
    f32 vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };

    u32 VBO;
    glGenBuffers(1, &VBO);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

i32 main()
{
    sf::RenderWindow window({1280, 720}, "Levox",
        sf::Style::Default, sf::ContextSettings(24, 8, 4, 3, 3));
    window.setActive();
    printContextSettings(window);

    if (!gladLoaderLoadGL())
    {
        std::cerr << "Failed to load OpenGL extension libraries" << std::endl;
        return 1;
    }

    if (!ImGui::SFML::Init(window))
    {
        std::cerr << "Failed to initialize ImGui" << std::endl;
        return 1;
    }

    sf::Clock deltaClock;
    bool isAppRunning = true;
    while (isAppRunning)
    {
        handleWindowEvents(window, isAppRunning);
        render_scene(window);
        render_imgui(window, deltaClock.restart());
        window.display();
    }

    window.setActive(false);
    ImGui::SFML::Shutdown();
    return 0;
}
