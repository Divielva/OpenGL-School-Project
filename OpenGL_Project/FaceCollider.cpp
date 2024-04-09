#include "FaceCollider.h"
#include "Heightmap.h"

glm::vec3 FaceCollider::get_height_at_coord(const glm::vec3& point) const
{
    return get_height_at_coord(glm::dvec3(point));
}

glm::vec3 FaceCollider::get_height_at_coord(const glm::dvec3& point) const
{
    auto vertices = parent->vertices;
    auto indices = parent->indices;

    for (auto i = 0; i < indices.size(); i += 3)
    {
        auto translate = glm::dvec3(parent->model[3]);
        auto a = glm::dvec3(vertices[indices[i]].position) + translate;
        auto b = glm::dvec3(vertices[indices[i + 1]].position) + translate;
        auto c = glm::dvec3(vertices[indices[i + 2]].position) + translate;

        auto v0 = glm::abs(b.xz() - a.xz());
        auto v1 = glm::abs(c.xz() - a.xz());
        auto v2 = glm::abs(point.xz() - a.xz());

        auto den = v0.x * v1.y - v1.x * v0.y;

        auto v = (v2.x * v1.y - v1.x * v2.y) / den;
        auto w = (v0.x * v2.y - v2.x * v0.y) / den;
        auto u = 1.0f - v - w;

        if (v >= 0 && w >= 0 && u >= 0)
        {
            auto height_a = a * u;
            auto height_b = b * v;
            auto height_c = c * w;

            auto height = height_a + height_b + height_c;

            auto error = height - point;

            return glm::vec3(point.x, height.y, point.z);
        }
    }

    return point;
}