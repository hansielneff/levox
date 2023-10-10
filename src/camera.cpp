#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 global_up,
    f32 FOV, f32 aspect, f32 near_plane, f32 far_plane, f32 sensitivity)
: position(position)
, target(target)
, global_up(global_up)
, FOV(FOV)
, aspect(aspect)
, near_plane(near_plane)
, far_plane(far_plane)
, sensitivity(sensitivity)
{
    calculateLocalVectors();
    assert(FOV > 0.0f);
}

void Camera::pan(f32 offsetX, f32 offsetY)
{
    glm::vec3 displacement = (right * offsetX + up * offsetY) * sensitivity;
    position += displacement;
    target += displacement;
}

#include <iostream>

void Camera::orbit(f32 degreesX, f32 degreesY)
{
    // This function implements "Arcball rotation" as explained by Samaursa here:
    // https://gamedev.stackexchange.com/questions/20758/how-can-i-orbit-a-camera-about-its-target-point

    // TODO: Surely there's a better way to rotate a vec3..
    f32 yaw = glm::radians(degreesX) * sensitivity;
    f32 pitch = glm::radians(degreesY) * sensitivity;
    glm::mat4 rotation = glm::mat4(1.0f);
    rotation = glm::rotate(rotation, yaw, up);
    rotation = glm::rotate(rotation, pitch, right);
    glm::vec4 intermediate = rotation * glm::vec4(targetToCamera, 1.0f);
    f32 normalizedPitch = intermediate.y / glm::length(targetToCamera);
    if (normalizedPitch < 0.99f && normalizedPitch > -0.99f)
    {
        targetToCamera.x = intermediate.x;
        targetToCamera.y = intermediate.y;
        targetToCamera.z = intermediate.z;
        targetToCamera += target;
        position = targetToCamera;
        calculateLocalVectors();
    }
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(position, target, up);
}

glm::mat4 Camera::getProjectionMatrix(ProjectionMode mode) const
{
    switch (mode)
    {
        case ProjectionMode::Perspective:
        {
            return glm::perspective(
                glm::radians(FOV), aspect, near_plane, far_plane);
        } break;
        case ProjectionMode::Orthographic:
        {
            f32 width = FOV * ORTHO_FOV_SCALAR * aspect;
            f32 height = FOV * ORTHO_FOV_SCALAR;
            return glm::ortho(
                position.x - width / 2, position.x + width / 2,
                position.y - height / 2, position.y + height / 2,
                near_plane + position.z, far_plane + position.z
            );
        } break;
    }
}

void Camera::calculateLocalVectors()
{
    targetToCamera = position - target;
    glm::vec3 forward = glm::normalize(targetToCamera);
    right = glm::normalize(glm::cross(global_up, forward));
    up = glm::cross(forward, right);

    assert(glm::length(forward) > 0.999f && glm::length(forward) < 1.001f);
    assert(glm::length(right) > 0.999f && glm::length(right) < 1.001f);
    assert(glm::length(up) > 0.999f && glm::length(up) < 1.001f);
}
