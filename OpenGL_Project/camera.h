#pragma once
#include "glm/fwd.hpp"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

class Camera
{
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    glm::vec3 eulerAngles;

    glm::mat4 viewMatrix;
    glm::mat4 inverseViewMatrix;

    float yaw;
    float pitch;

    void update_camera_vectors();

public:
    Camera();
    Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);
    void set_position(glm::vec3 position);
    void set_rotation(glm::vec3 euler);
    glm::mat4 get_view_matrix() const;
    glm::mat4 get_inverse_view_matrix() const;
    glm::vec3 get_view_pos() const;
};


