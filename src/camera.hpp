#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "types.hpp"

#define DEFAULT_SENSITIVITY_PAN 0.025f
#define DEFAULT_SENSITIVITY_ORBIT 0.2f
#define DEFAULT_SENSITIVITY_ZOOM 2.0f

class Camera
{
public:
    bool perspective;
    f32 FOV;
    f32 aspect;
    f32 near_plane;
    f32 far_plane;
    f32 sensitivity_pan;
    f32 sensitivity_orbit;
    f32 sensitivity_zoom;

    Camera(
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 global_up = glm::vec3(0.0f, 1.0f, 0.0f),
        bool perspective = true,
        f32 FOV = 45.0f,
        f32 aspect = 16.0f / 9.0f,
        f32 near_plane = 0.1f,
        f32 far_plane = 100.0f
    );

    void pan(f32 degreesX, f32 degreesY);
    void orbit(f32 degreesX, f32 degreesY);
    void zoom(f32 amount);

    glm::vec3 screenToWorld(i32 x, i32 y, i32 width, i32 height, bool yUp = false);

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
    glm::vec3 getLookDir() const;

private:
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 targetToCamera;
    glm::vec3 global_up;
    glm::vec3 forward;
    glm::vec3 up;
    glm::vec3 right;

    void calculateLocalVectors();
};

#endif
