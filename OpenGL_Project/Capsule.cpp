#include "Capsule.h"

Capsule::Capsule()
{
    position = glm::vec3(0, 0, 0);
    radius = .5f;
    height = 1;
    rotation = glm::quat(1, 0, 0, 0);
    subdivision = 0;
    generate_vertices();
}

void Capsule::generate_vertices()
{
    vertices.clear();
    indices.clear();
    vertices.reserve(10);
    Vertex vertex = Vertex();
    Vertex new_v1, new_v2, new_v3, v1, v2, v3;
    float radius_mod = radius;
    vertex.position.z = radius_mod;
    vertex.normal = glm::vec3(0, 0, 1);
    vertices.push_back(vertex); // 0
    vertex.position.z = .000001f;
    vertex.position.y = radius_mod;
    vertex.normal = glm::vec3(0, 1, 0);
    vertices.push_back(vertex); // 1
    vertex.position.x = radius_mod;
    vertex.position.y = 0;
    vertex.normal = glm::vec3(1, 0, 0);
    vertices.push_back(vertex); // 2
    vertex.position.y = -radius_mod;
    vertex.position.x = 0;
    vertex.normal = glm::vec3(0, -1, 0);
    vertices.push_back(vertex); // 3
    vertex.position.x = -radius_mod;
    vertex.position.y = 0;
    vertex.normal = glm::vec3(-1, 0, 0);
    vertices.push_back(vertex); // 4
    vertex.position.z = -.000001f;
    vertex.position.y = radius_mod;
    vertex.position.x = 0;
    vertex.normal = glm::vec3(0, 0, 1);
    vertices.push_back(vertex); // 5
    vertex.position.x = radius_mod;
    vertex.position.y = 0;
    vertex.normal = glm::vec3(1, 0, 0);
    vertices.push_back(vertex); // 6
    vertex.position.y = -radius_mod;
    vertex.position.x = 0;
    vertex.normal = glm::vec3(0, -1, 0);
    vertices.push_back(vertex); // 7
    vertex.position.x = -radius_mod;
    vertex.position.y = 0;
    vertex.normal = glm::vec3(-1, 0, 0);
    vertices.push_back(vertex); // 8
    vertex.position.x = 0;
    vertex.position.z = -radius_mod;
    vertex.normal = glm::vec3(0, 0, -1);
    vertices.push_back(vertex); // 9

    indices = {
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 1,
        1, 2, 5,
        2, 6, 5,
        6, 2, 3,
        3, 7, 6,
        7, 3, 4,
        4, 8, 7,
        8, 4, 1,
        1, 5, 8,
        9, 5, 6,
        9, 6, 7,
        9, 7, 8,
        9, 8, 5 };

    if (subdivision > 0)
    {
        std::vector<Vertex> old_vertex;
        std::vector<unsigned> old_indices;
        unsigned index = 0;
        for (unsigned i = 0; i < subdivision; ++i)
        {
            old_vertex = vertices;
            old_indices = indices;
            vertices.clear();
            indices.clear();
            index = 0;

            for (unsigned j = 0; j < old_indices.size(); j += 3)
            {
                v1 = old_vertex[old_indices[j]];
                v2 = old_vertex[old_indices[j + 1]];
                v3 = old_vertex[old_indices[j + 2]];

                compute_half_vertex(v1, v2, new_v1);
                compute_half_vertex(v2, v3, new_v2);
                compute_half_vertex(v1, v3, new_v3);

                push_vector(vertices, v1, new_v1, new_v3);
                push_vector(vertices, new_v1, v2, new_v2);
                push_vector(vertices, new_v1, new_v2, new_v3);
                push_vector(vertices, new_v3, new_v2, v3);

                for (char k = 0; k < 4; ++k)
                {
                    push_vector(indices, index, index + 1, index + 2);
                    index += 3;
                }
            }
        }
    }

    for (auto& vertex : vertices)
    {
        if (height - radius > 0)
        {
            if (vertex.position.z > 0)
                vertex.position.z += height - radius;
            else
                vertex.position.z -= height - radius;
        }

        vertex.position = rotation * vertex.position;
        vertex.position += position;
    }
}

void Capsule::compute_half_vertex(const Vertex& a, const Vertex& b, Vertex& result)
{
    result.normal = normalize(a.normal + b.normal);
    result.position = normalize(a.position + b.position);
    result.position *= radius / length(result.position);
}

void Capsule::set_radius(const float radius)
{
    this->radius = radius;
    vertices.clear();
    generate_vertices();
}

void Capsule::set_height(const float height)
{
    this->height = height;
    vertices.clear();
    generate_vertices();
}

void Capsule::set_subdivision(const unsigned int subdivision)
{
    this->subdivision = subdivision;
    vertices.clear();
    generate_vertices();
}

void Capsule::set_aabb(const glm::vec3& center, const glm::vec3& size)
{
    aabb.center = center;
    aabb.extent = size * glm::vec3(radius / 2, height / 2, radius / 2);
}