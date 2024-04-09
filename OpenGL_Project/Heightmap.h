#pragma once
#include "ITriangleObject.h"
#include "FaceCollider.h"

class Heightmap : public ITriangleObject
{
    FaceCollider collider;

public:
    void generate_terrain(const char* heightmap_path, const float& height_scale, const float& texture_scale);
    FaceCollider& get_collider() { return collider; }
};