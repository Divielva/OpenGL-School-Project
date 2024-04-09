#include "Character.h"

#include "Math.h"
#include "ObjectBuffer.h"
#include "OBB.h"
#include "glm/gtx/rotate_normalized_axis.inl"
#include "glm/gtx/rotate_vector.hpp"
#include "Material.h"

#define MOVEMENT_SPEED 5000.f
#define MOUSE_SENSITIVITY 0.1f
#define PITCH_CONSTRAINT 89.0f

Character::Character()
{
    model = Capsule();
    //model1 = Sphere();  
    look = Cone();
    camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
    pitch = -20.0f;
    set_position(glm::vec3(0.0f, 0.0f, 0.0f));
    model.set_subdivision(3);
    look.set_subdivision(3);
    look.set_color(hsl(235, 1.f, 0.5f));
    model.set_albedo(hsl(235, 1.f, 0.5f));
    model.set_euler_rotation(glm::vec3(glm::radians(90.f), 0, 0));
    model.set_height(height);
    model.set_radius(radius);
    aabb = AABB(glm::vec3(0), glm::vec3(radius, height / 2, radius));
    look.set_height(height / 2);
    look.set_radius(radius / 2);
    collision = new OBB();
    update_sub_objects();
}

glm::vec3 Character::get_camera_position() const
{
    glm::vec3 ret;
    ret.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    ret.y = sin(glm::radians(pitch));
    ret.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    ret = normalize(ret);
    ret *= cameraOffset;
    return position - ret + glm::vec3(0, .5, 0);
}

void Character::update_sub_objects()
{
    auto modelPos = position + glm::vec3(0, height / 2, 0);
    model.set_position(modelPos);
    aabb.center = modelPos;
    const auto zRotMat = rot_z_mat(90.f);
    const auto xRotMat = rot_x_mat(-yaw);
    look.set_position(position + rotateY(glm::vec3(.45f, 0, 0), glm::radians(-yaw)) + glm::vec3(0, 0.45, 0));
    look.set_rotation(combine(&zRotMat, &xRotMat, nullptr));
    if (!cameraExplicit)
    {
        camera.set_position(get_camera_position());
        camera.set_rotation(glm::vec3(pitch, yaw, 0));
    }
    if (collision)
    {
        dynamic_cast<OBB*>(collision)->points = {
            glm::vec2(-radius, -radius),
            glm::vec2(radius, -radius),
            glm::vec2(-radius, radius),
            glm::vec2(radius, radius) };
        collision->set_position(glm::vec2(position.x, position.z));
        collision->set_rotation(yaw);
    }
}

void Character::set_material(Material* material)
{
    model.material = material;
    look.material = material;
    material = material;
}

void Character::set_shader(Shader* shader)
{
    model.shader = shader;
    look.shader = shader;
}

void Character::set_explicit_camera(const glm::vec3& position, const glm::vec3 euler, const bool was_explicit)
{
    cameraExplicit = was_explicit;
    camera.set_position(position);
    camera.set_rotation(euler);
}

void Character::set_position(const glm::vec3& position)
{
    this->position = position;
    aabb.center = position;
    update_sub_objects();
}

void Character::set_look(const float yaw, const float pitch)
{
    this->yaw = yaw;
    this->pitch = pitch;
    update_sub_objects();
}

void Character::process_mouse_movement(const double x_offset, const double y_offset, const bool constrain_pitch)
{
    yaw += static_cast<float>(x_offset * MOUSE_SENSITIVITY);
    pitch += static_cast<float>(y_offset * MOUSE_SENSITIVITY);
    if (constrain_pitch)
    {
        if (pitch > PITCH_CONSTRAINT)
            pitch = PITCH_CONSTRAINT;
        if (pitch < -PITCH_CONSTRAINT)
            pitch = -PITCH_CONSTRAINT;
    }
    if (yaw > 180.0f)
        yaw -= 360.0f;
    if (yaw < -180.0f)
        yaw += 360.0f;
    update_sub_objects();
}

void Character::process_mouse_scroll(const double y_offset)
{
    cameraOffset -= static_cast<float>(y_offset);
    if (cameraOffset < 1.0f)
        cameraOffset = 1.0f;
    if (cameraOffset > 10.0f)
        cameraOffset = 10.0f;

    camera.set_position(get_camera_position());
}

void Character::update_position(const glm::vec3& direction, const double delta_time, const ObjectBuffer& buffer)
{
    const double velocityD = MOVEMENT_SPEED * delta_time;
    const float velocity = static_cast<float>(velocityD);
    const auto newPos = position + rotateY(direction, glm::radians(-yaw)) * velocity;
    const auto objsInRange = buffer.get_objects_in_range(newPos, 10);
    collision->set_position(glm::vec2(newPos.x, newPos.z));
    aabb.center = newPos;
    if (!objsInRange.empty())
        for (const auto& obj : objsInRange)
            for (const auto& objAabb : obj->get_aabbs())
                if (objAabb != nullptr && objAabb->intersect(aabb, nullptr))
                {
                    aabb.center = position;
                    return;
                }

    position = newPos;
    update_sub_objects();
}

void Character::check_overlap(const ObjectBuffer& buffer) const
{
    const auto objsInRange = buffer.get_objects_in_range(position, 10);
    if (!objsInRange.empty())
        for (const auto& obj : objsInRange)
            for (const auto& objCollision : obj->get_collisions())
                if (objCollision != nullptr)
                    objCollision->check_overlap(collision);
}

void Character::draw_shadow()
{
    model.draw(ShaderStore::get_shader("shadowMap"));
}

void Character::draw()
{
    model.draw();
    look.draw();
}

glm::vec3 Character::get_position() const
{
    return position;
}

glm::quat Character::get_look() const
{
    return glm::vec3(pitch, yaw, 0);
}

glm::vec2 Character::get_look_angles() const
{
    return { pitch, yaw };
}

void Character::update_shader(const Shader* shader) const
{
    shader->set_mat4("view", camera.get_view_matrix());
    shader->set_vec3("viewPos", camera.get_view_pos());
}

Character::~Character()
{
    delete collision;
}

glm::mat4 Character::get_view() const
{
    return camera.get_view_matrix();
}