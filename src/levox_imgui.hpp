#ifndef LEVOX_IMGUI_H
#define LEVOX_IMGUI_H

#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

void initImGui(sf::RenderWindow &window);
void renderImGui(sf::RenderWindow &window, sf::Time delta);

#endif
