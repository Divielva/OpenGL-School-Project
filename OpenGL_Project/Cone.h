#pragma once
#include "HSL.h"
#include "ITriangleObject.h"

class Cone final : public ITriangleObject
{
    glm::vec3 position;
    glm::quat rotation;
    float radius;
    float height;
    unsigned subdivision = 0;
    hsl color;
    void generate_vertices();
    void compute_half_vertex(const Vertex& a, const Vertex& b, Vertex& result) const;

public:
    Cone();
    void set_color(const hsl& color);
    void set_position(glm::vec3 position);
    void set_radius(float radius);
    void set_height(float height);
    void set_subdivision(unsigned int subdivision);
    void set_rotation(glm::quat quaternion);
    void set_euler_rotation(glm::vec3 angle);
    void pre_draw() override;
};