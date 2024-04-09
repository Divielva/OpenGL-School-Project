#pragma once

#include "HSL.h"
#include "ITriangleObject.h"

class Plane : public ITriangleObject
{
    glm::vec3 position;
    glm::vec2 size;
    hsl color;
    glm::quat rotation;
    void generate_vertices();

public:
    Plane();
    void set_size(glm::vec2 size);
};