#include "ObjectBuffer.h"

#include "glad/glad.h"
#include <glm/gtx/norm.hpp>
#include "ShaderStore.h"

ObjectBuffer::~ObjectBuffer()
{
    vertices.clear();
    indices.clear();
    objects.clear();
}

void ObjectBuffer::init_buffers()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &ebo);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glEnableVertexAttribArray(0);
    // ReSharper disable CppCStyleCast
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture_coord));
 
    // ReSharper restore CppCStyleCast
    glBindVertexArray(0);
}

void ObjectBuffer::draw(bool depth_draw) const
{
    glBindVertexArray(vao);
    for (const auto& object : objects)
    {
        if (object->no_depth && depth_draw)
            continue;
        object->draw(depth_draw ? ShaderStore::get_shader("shadowMap") : nullptr);
    }
}

void ObjectBuffer::add_object(IObject* object)
{
    objects.push_back(object);
}

void ObjectBuffer::destroy_object(IObject* object)
{
    const auto it = std::find(objects.begin(), objects.end(), object);
    if (it != objects.end())
    {
        objects.erase(it);
    }
}

std::vector<IObject*> ObjectBuffer::get_objects() const
{
    return objects;
}

std::vector<IObject*> ObjectBuffer::get_objects_in_range(glm::vec3 position, float radius) const
{
    std::vector<IObject*> objectsInRange;
    for (const auto& object : objects)
    {
        glm::vec3 min, max;
        if (object->vertices.empty())
            continue;
        min = max = object->vertices[0].position;
        for (auto vertex : object->vertices)
        {
            if (vertex.position <= min)
            {
                min = vertex.position;
            }
            if (vertex.position >= max)
            {
                max = vertex.position;
            }
        }
        auto objectCenter = (min + max) / 2.0f;
        auto distance = glm::distance2(position, objectCenter);
        if (distance <= radius * radius)
        {
            objectsInRange.push_back(object);
        }
    }
    return objectsInRange;
}
