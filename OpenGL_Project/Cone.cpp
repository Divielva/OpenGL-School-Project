#include "Cone.h"

Cone::Cone()
{
    this->position = glm::vec3(0.f);
    this->rotation = glm::vec3(0.f);
    this->color = hsl(0, 1, 0.5);
    height = 1;
    radius = 0.5f;
}

void Cone::generate_vertices()
{
    Vertex v;
    v.normal = color.get_rgb_vec3();
    v.position.y = height;
    vertices.push_back(v);
    v.position.y = 0;
    v.position.x = radius;
    vertices.push_back(v);
    v.position.z = -radius;
    v.position.x = 0;
    vertices.push_back(v);
    v.position.x = -radius;
    v.position.z = 0;
    vertices.push_back(v);
    v.position.x = 0;
    v.position.z = radius;
    vertices.push_back(v);

    indices = {
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 1
    };

    Vertex new_v1, new_v2, new_v3, v1, v2, v3;
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
            vertices.push_back(old_vertex[0]);
            index = 1;

            for (unsigned j = 0; j < old_indices.size(); j += 3)
            {
                v2 = old_vertex[old_indices[j + 1]];
                v3 = old_vertex[old_indices[j + 2]];

                compute_half_vertex(v2, v3, new_v1);

                push_vector(vertices, v2, new_v1);
                push_vector(vertices, new_v1, v3);

                for (char k = 0; k < 2; ++k)
                {
                    push_vector(indices, old_indices[0], index, index + 1);
                    index += 2;
                }
            }
        }
    }

    for (auto& vertex : vertices)
    {
        vertex.position = rotation * vertex.position;
        vertex.position += position;
    }
}

void Cone::compute_half_vertex(const Vertex& a, const Vertex& b, Vertex& result) const
{
    result.normal = a.normal;
    result.position = normalize(a.position + b.position);
    result.position *= radius / length(result.position);
}


void Cone::set_color(const hsl& color)
{
    this->color = color;
    vertices.clear();
}

void Cone::set_position(const glm::vec3 position)
{
    this->position = position;
    vertices.clear();
}

void Cone::set_radius(const float radius)
{
    this->radius = radius;
    vertices.clear();
}

void Cone::set_height(const float height)
{
    this->height = height;
    vertices.clear();
}

void Cone::set_subdivision(const unsigned int subdivision)
{
    this->subdivision = subdivision;
    vertices.clear();
}

void Cone::set_rotation(const glm::quat quaternion)
{
    this->rotation = eulerAngles(quaternion);
    vertices.clear();
}

void Cone::set_euler_rotation(const glm::vec3 angle)
{
    this->rotation = angle;
    vertices.clear();
}

void Cone::pre_draw()
{
    if (vertices.empty())
        generate_vertices();
}