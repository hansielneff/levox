#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

#include <SFML/Window.hpp>

#include "camera.hpp"
#include "voxel_mesh.hpp"
#include "toolbox.hpp"

void handleWindowEvents(sf::Window &window, Camera &camera,
    VoxelMesh &vertexMesh, Toolbox &toolbox, bool &isAppRunning);

#endif
