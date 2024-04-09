#pragma once
#include "ICollider.h"

class Heightmap;

struct FaceCollider : public ICollisionOld
{
    Heightmap* parent = nullptr;
    FaceCollider() = default;
    FaceCollider(IObject* parent) : ICollisionOld(parent)
    {
    }

    FaceCollider(const FaceCollider&) = default;
    FaceCollider(FaceCollider&&) = default;
    FaceCollider& operator=(const FaceCollider&) = default;
    FaceCollider& operator=(FaceCollider&&) = default;

    glm::vec3 get_height_at_coord(const glm::dvec3& point) const;
    glm::vec3 get_height_at_coord(const glm::vec3& point) const;
};