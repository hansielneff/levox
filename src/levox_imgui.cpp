#include <iostream>

#include "levox_imgui.hpp"

void initImGui(sf::RenderWindow &window)
{
    if (!ImGui::SFML::Init(window))
        throw std::runtime_error("Error: Failed to initialize ImGui");
}

void renderImGui(sf::RenderWindow &window, sf::Time delta)
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
