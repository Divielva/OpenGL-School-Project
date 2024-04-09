#include "Plane.h"

Plane::Plane()
{
    position = glm::vec3(0.0f);
    size = glm::vec2(1.0f);
    color = hsl(0.0f, 1.0f, 0.5f);
    rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    generate_vertices();
}

void Plane::generate_vertices()
{
    vertices = {
        Vertex(glm::vec3(-size.x / 2, 0, -size.y / 2), glm::vec3(0, 1, 0), glm::vec2(0, 0)),
        Vertex(glm::vec3(size.x / 2, 0, -size.y / 2), glm::vec3(0, 1, 0), glm::vec2(1, 0)),
        Vertex(glm::vec3(size.x / 2, 0, size.y / 2), glm::vec3(0, 1, 0), glm::vec2(1, 1)),
        Vertex(glm::vec3(-size.x / 2, 0, size.y / 2), glm::vec3(0, 1, 0), glm::vec2(0, 1)) };
    indices = { 0, 1, 2, 0, 2, 3 };
}

void Plane::set_size(glm::vec2 size)
{
    this->size = size;
    generate_vertices();
}