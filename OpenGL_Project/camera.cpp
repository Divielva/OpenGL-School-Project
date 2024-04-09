#include "camera.h"

#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/rotate_vector.hpp"

Camera::Camera() : Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0, 0)
{
}

Camera::Camera(const glm::vec3 position, const glm::vec3 up, const float yaw, const float pitch)
{
    this->position = position;
    this->worldUp = up;
    this->yaw = yaw;
    this->pitch = pitch;
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    update_camera_vectors();
}

void Camera::update_camera_vectors()
{
    // calculate the new front vector
    glm::vec3 new_front;
    new_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    new_front.y = sin(glm::radians(pitch));
    new_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    auto new_up = glm::vec3(0.0f, 1.0f, 0.0f);
    new_up = rotateZ(new_up, glm::radians(eulerAngles.z));
    front = normalize(new_front);
    // also re-calculate the right and up vector
    right = normalize(cross(front, new_up));
    up = normalize(cross(right, front));
    this->viewMatrix = lookAt(this->position, this->position + this->front, this->up);
    this->inverseViewMatrix = inverse(this->viewMatrix);
}

glm::mat4 Camera::get_view_matrix() const
{
    return viewMatrix;
}

glm::mat4 Camera::get_inverse_view_matrix() const
{
    return inverseViewMatrix;
}

void Camera::set_position(const glm::vec3 position)
{
    this->position = position;
    this->viewMatrix = lookAt(this->position, this->position + this->front, this->up);
    this->inverseViewMatrix = inverse(this->viewMatrix);
}

void Camera::set_rotation(glm::vec3 euler)
{
    this->yaw = euler.y;
    this->pitch = euler.x;
    this->eulerAngles = euler;
    update_camera_vectors();
}

glm::vec3 Camera::get_view_pos() const
{
    return position;
}