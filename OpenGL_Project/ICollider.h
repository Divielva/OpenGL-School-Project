#pragma once
#include "glm/vec3.hpp"
#include "glm/gtc/quaternion.hpp"
#include <vector>

class IObject;
// we are ignoring height for now
constexpr glm::vec3 reboundMultiplier = glm::vec3(1, 0, 1);

struct ICollisionOld
{
    virtual ~ICollisionOld() = default;
    ICollisionOld() = default;

    explicit ICollisionOld(IObject* parent) : parent(parent)
    {
    }

    ICollisionOld(const ICollisionOld&) = default;
    ICollisionOld(ICollisionOld&&) = default;
    ICollisionOld& operator=(const ICollisionOld&) = default;
    ICollisionOld& operator=(ICollisionOld&&) = default;
    glm::vec2 position = glm::vec2(0);
    float angle = 0;
    IObject* draw_object = nullptr;
    bool should_overlap = false;
    void (*on_collision)(IObject* parent_self, ICollisionOld* self, ICollisionOld* other) = nullptr;
    IObject* parent = nullptr;

    virtual glm::vec2 get_center() const
    {
        return position;
    }

    void set_position(const glm::vec2& position)
    {
        this->position = position;
    }

    void set_rotation(const float angle)
    {
        this->angle = angle;
    }

    virtual bool contains(ICollisionOld* collider)
    {
        return false;
    }

    virtual void check_overlap(ICollisionOld* collider)
    {
    }

    virtual glm::vec3 rebound(const glm::vec3 point, const glm::vec3 velocity) const
    {
        return point;
    }

    glm::vec3 rebound_with_scalar(const glm::vec3 point, const glm::vec3 velocity) const
    {
        return rebound(point, velocity) * reboundMultiplier;
    }

    glm::vec3 rebound_with_scalar(const glm::vec3 point) const
    {
        return rebound_with_scalar(point, glm::vec3(0));
    }

    virtual void draw_bounds()
    {
    }

    virtual std::vector<glm::vec2> get_points()
    {
        return { position };
    }
};

class ICollider
{
    virtual glm::vec3 find_furthest_point(const glm::vec3& direction) const = 0;
};

enum class CollisionType
{
    STATIC,
    DYNAMIC,
    KINEMATIC
};

enum class CollisionResponse
{
    BLOCK,
    OVERLAP,
    IGNORE
};

struct CollisionProperties
{
    CollisionType type = CollisionType::STATIC;
    CollisionResponse response = CollisionResponse::BLOCK;

    bool is_static() const { return type == CollisionType::STATIC; }
    bool is_dynamic() const { return type == CollisionType::DYNAMIC; }
    bool is_kinematic() const { return type == CollisionType::KINEMATIC; }

    bool should_block() const { return response == CollisionResponse::BLOCK; }
    bool should_overlap() const { return response == CollisionResponse::OVERLAP; }
    bool should_ignore() const { return response == CollisionResponse::IGNORE; }
};