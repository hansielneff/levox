#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SFML/Graphics.hpp>

#include "camera.hpp"

void initOpenGLContext();
void renderModel(sf::RenderWindow &window, Camera &camera);

#endif
