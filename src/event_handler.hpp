#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

#include <SFML/Window.hpp>

#include "camera.hpp"
#include "voxel_mesh.hpp"

void handleWindowEvents(sf::Window &window, Camera &camera,
    VoxelMesh &vertexMesh, bool &isAppRunning);

#endif
