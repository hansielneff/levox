#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 global_up,
    bool perspective, f32 FOV, f32 aspect, f32 near_plane, f32 far_plane)
: position(position)
, target(target)
, global_up(global_up)
, perspective(perspective)
, FOV(FOV)
, aspect(aspect)
, near_plane(near_plane)
, far_plane(far_plane)
, sensitivity_pan(SENSITIVITY_PAN_DEFAULT)
, sensitivity_orbit(SENSITIVITY_ORBIT_DEFAULT)
{
    calculateLocalVectors();
    assert(FOV > 0.0f);
}

void Camera::pan(f32 offsetX, f32 offsetY)
{
    glm::vec3 displacement = (right * -offsetX + up * offsetY) * sensitivity_pan;
    position += displacement;
    target += displacement;
}

void Camera::orbit(f32 degreesX, f32 degreesY)
{
    // This function implements "Arcball rotation" as explained by Samaursa here:
    // https://gamedev.stackexchange.com/questions/20758/how-can-i-orbit-a-camera-about-its-target-point

    // TODO: Surely there's a better way to rotate a vec3..
    f32 yaw = glm::radians(-degreesX) * sensitivity_orbit;
    f32 pitch = glm::radians(-degreesY) * sensitivity_orbit;
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

glm::mat4 Camera::getProjectionMatrix() const
{
    if (perspective)
    {
        return glm::perspective(
            glm::radians(FOV), aspect, near_plane, far_plane);
    }
    else
    {
        f32 targetDistance = glm::length(targetToCamera);
        f32 width = targetDistance * aspect;
        f32 height = targetDistance;
        return glm::ortho(
            -width / 2, width / 2,
            -height / 2, height / 2,
            near_plane, far_plane
        );
    }
}

void Camera::calculateLocalVectors()
{
    targetToCamera = position - target;
    glm::vec3 forward = glm::normalize(targetToCamera);
    right = glm::normalize(glm::cross(global_up, forward));
    up = glm::cross(forward, right);

    assert(glm::length(forward) > 0.99f && glm::length(forward) < 1.01f);
    assert(glm::length(right) > 0.99f && glm::length(right) < 1.01f);
    assert(glm::length(up) > 0.99f && glm::length(up) < 1.01f);
}
