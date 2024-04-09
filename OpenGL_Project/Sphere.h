#pragma once
#include "HSL.h"
#include "ITriangleObject.h"

class Sphere final : public ITriangleObject
{
    hsl color;
    glm::vec3 position;
    float radius;
    glm::quat rotation;
    unsigned int subdivision = 0;
    void generate_vertices();
    void compute_half_vertex(const Vertex& a, const Vertex& b, Vertex& result) const;

public:
    Sphere();
    void set_color(const hsl& color);
    void set_position(glm::vec3 position);
    void set_radius(float radius);
    void set_subdivision(unsigned int subdivision);
    void set_rotation(glm::quat quaternion);
    void set_euler_rotation(glm::vec3 angle);
    void pre_draw() override;
};