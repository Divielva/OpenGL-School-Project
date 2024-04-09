#include "Cube.h"
#include <array>

Cube::Cube()
{
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    scale = glm::vec3(1.0f, 1.0f, 1.0f);
    rotation = glm::quat(1, 0, 0, 0);
    draw_mode = GL_TRIANGLES;
    generate_vertices();
}

void Cube::push_face(std::vector<Vertex> face, std::vector<unsigned> indices)
{
    auto start = vertices.size();
    for (auto& v : face)
    {
        vertices.push_back(v);
    }
    for (auto& i : indices)
    {
        this->indices.push_back(start + i);
    }
}

void Cube::generate_vertices()
{
    Vertex v0, v1, v2, v3;
    v0 = Vertex(glm::vec3(1), glm::vec3(0, 0, 1));
    memcpy(&v1, &v0, sizeof(Vertex));
    memcpy(&v2, &v0, sizeof(Vertex));
    memcpy(&v3, &v0, sizeof(Vertex));

    // set the correct position for each vertex
    v1.position.x = -1;
    v2.position.y = -1;
    v2.position.x = -1;
    v3.position.y = -1;

    glm::quat y = glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0));
    glm::quat x = glm::angleAxis(glm::radians(90.0f), glm::vec3(1, 0, 0));

    std::vector<Vertex> tmpVertices = { v0, v1, v2, v3 };
    std::vector<unsigned> tmpIndices = { 0, 3, 1, 3, 2, 1 };
    vertices.reserve(24);
    indices.reserve(36);

    tmpVertices[0].texture_coord = glm::vec2(0, 0);
    tmpVertices[1].texture_coord = glm::vec2(1, 0);
    tmpVertices[2].texture_coord = glm::vec2(1, 1);
    tmpVertices[3].texture_coord = glm::vec2(0, 1);
    for (int i = 0; i < 4; i++)
    {
        for (auto& v : tmpVertices)
        {
            v.position = y * v.position;
            v.normal = y * v.normal;
        }
        push_face(tmpVertices, tmpIndices);
    }
    for (auto& v : tmpVertices)
    {
        v.position = x * v.position;
        v.normal = x * v.normal;
    }
    push_face(tmpVertices, tmpIndices);
    for (int i = 0; i < 2; i++)
    {
        for (auto& v : tmpVertices)
        {
            v.position = x * v.position;
            v.normal = x * v.normal;
        }
    }
    push_face(tmpVertices, tmpIndices);
}

void Cube::pre_draw()
{
    if (vertices.empty())
        generate_vertices();
}