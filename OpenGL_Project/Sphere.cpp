#include "Sphere.h"

Sphere::Sphere()
{
    color = hsl(0, 1, 0.5);
    position = glm::vec3(0, 0, 0);
    radius = 1;
    rotation = glm::quat(1, 0, 0, 0);
    subdivision = 0;
    generate_vertices();
}

void Sphere::generate_vertices()
{
    vertices.clear();
    indices.clear();
    vertices.reserve(6);
    Vertex vertex;
    vertex.position.z = radius;
    vertices.push_back(vertex);
    vertex.position.z = 0;
    vertex.position.y = radius;
    vertices.push_back(vertex);
    vertex.position.x = radius;
    vertex.position.y = 0;
    vertices.push_back(vertex);
    vertex.position.y = -radius;
    vertex.position.x = 0;
    vertices.push_back(vertex);
    vertex.position.x = -radius;
    vertex.position.y = 0;
    vertices.push_back(vertex);
    vertex.position.x = 0;
    vertex.position.z = -radius;
    vertices.push_back(vertex);

    indices = {
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 1,
        5, 1, 2,
        5, 2, 3,
        5, 3, 4,
        5, 4, 1
    };

    if (subdivision > 0)
    {
        Vertex newV1, newV2, newV3, v1, v2, v3;
        std::vector<Vertex> oldVertex;
        std::vector<unsigned> oldIndices;
        unsigned index = 0;
        for (unsigned i = 0; i < subdivision; ++i)
        {
            oldVertex = vertices;
            oldIndices = indices;
            vertices.clear();
            indices.clear();
            index = 0;

            for (unsigned j = 0; j < oldIndices.size(); j += 3)
            {
                v1 = oldVertex[oldIndices[j]];
                v2 = oldVertex[oldIndices[j + 1]];
                v3 = oldVertex[oldIndices[j + 2]];

                compute_half_vertex(v1, v2, newV1);
                compute_half_vertex(v2, v3, newV2);
                compute_half_vertex(v1, v3, newV3);

                push_vector(vertices, v1, newV1, newV3);
                push_vector(vertices, newV1, v2, newV2);
                push_vector(vertices, newV1, newV2, newV3);
                push_vector(vertices, newV3, newV2, v3);

                for (char k = 0; k < 4; ++k)
                {
                    push_vector(indices, index, index + 1, index + 2);
                    index += 3;
                }
            }
        }
    }

    for (auto& vert : vertices)
    {
        vert.normal = this->color.get_rgb_vec3();
        vert.position = rotation * vert.position;
        vert.position += position;
    }
}

void Sphere::compute_half_vertex(const Vertex& a, const Vertex& b, Vertex& result) const
{
    result.position = normalize(a.position + b.position);
    result.position *= radius / length(result.position);
}

void Sphere::set_color(const hsl& color)
{
    this->color = color;
    vertices.clear();
}

void Sphere::set_euler_rotation(const glm::vec3 angle)
{
    rotation = glm::quat(angle);
    vertices.clear();
}

void Sphere::set_position(const glm::vec3 position)
{
    this->position = position;
    vertices.clear();
}

void Sphere::set_radius(const float radius)
{
    this->radius = radius;
    vertices.clear();
}

void Sphere::set_rotation(const glm::quat quaternion)
{
    rotation = quaternion;
    vertices.clear();
}

void Sphere::set_subdivision(const unsigned int subdivision)
{
    this->subdivision = subdivision;
    vertices.clear();
}

void Sphere::pre_draw()
{
    if (vertices.empty())
        generate_vertices();
}