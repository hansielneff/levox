#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <SFML/Window.hpp>

#include "camera.h"

void handleWindowEvents(sf::Window &window, Camera &camera, bool &isAppRunning);

#endif
