#pragma once
#include "camera.h"
#include "Capsule.h"
#include "Sphere.h" 
#include "Cone.h"   
#include "AABB.h"
#include "glm/vec3.hpp"

class ObjectBuffer;
struct Material;

class Character
{
    glm::vec3 position;
    float yaw;
    float pitch;
    float cameraOffset = 2.0f;
    float height = .5f;
    float radius = .5f;
    Material* material;
    Capsule model;
    //Sphere model;
    Cone look;
    Camera camera;
    ICollisionOld* collision;
    AABB aabb;
    bool cameraExplicit = false;

    void update_sub_objects();

public:
    Character();
    ~Character();
    void set_explicit_camera(const glm::vec3& position, glm::vec3 euler, bool was_explicit);
    void set_position(const glm::vec3& position);
    void set_look(float yaw, float pitch);
    void process_mouse_movement(double x_offset, double y_offset, bool constrain_pitch = true);
    void process_mouse_scroll(double y_offset);
    void set_shader(Shader* shader);
    void update_shader(const Shader* shader) const;
    void update_position(const glm::vec3& direction, double delta_time, const ObjectBuffer& buffer);
    void check_overlap(const ObjectBuffer& buffer) const;
    void set_material(Material* material);
    void draw();
    void draw_shadow();
    glm::vec3 get_camera_position() const;
    glm::vec3 get_position() const;
    glm::quat get_look() const;
    glm::vec2 get_look_angles() const;
    glm::mat4 get_view() const;
};
