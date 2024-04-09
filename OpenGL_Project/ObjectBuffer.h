#pragma once
#include <vector>

#include "IObject.h"

class ObjectBuffer
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<IObject*> objects;
    unsigned int vao;
    unsigned int ebo;
    unsigned int vbo;

public:
    ObjectBuffer() = default;
    ObjectBuffer(const ObjectBuffer&) = delete;
    ObjectBuffer(ObjectBuffer&&) = delete;
    ObjectBuffer& operator=(const ObjectBuffer&) = delete;
    ObjectBuffer& operator=(ObjectBuffer&&) = delete;

    void init_buffers();
    void draw(bool depth_draw = false) const;
    void add_object(IObject* object);
    void destroy_object(IObject* object);
    std::vector<IObject*> get_objects_in_range(glm::vec3 position, float radius) const;
    std::vector<IObject*> get_objects() const;

    ~ObjectBuffer();
};
