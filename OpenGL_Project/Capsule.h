#pragma once
#include "HSL.h"
#include "ITriangleObject.h"

class Capsule final : public ITriangleObject
{
    float radius;
    float height;
    unsigned int subdivision = 0;
    void generate_vertices();
    void compute_half_vertex(const Vertex& a, const Vertex& b, Vertex& result);

public:
    Capsule();
    void set_radius(float radius);
    void set_height(float height);
    void set_subdivision(unsigned int subdivision);
    void set_aabb(const glm::vec3& center, const glm::vec3& size) override;
};