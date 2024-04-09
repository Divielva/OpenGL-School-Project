#pragma once
#include "HSL.h"
#include "glm/fwd.hpp"

#include "IObject.h"

class Cube final : public IObject
{
    void generate_vertices();
    void push_face(std::vector<Vertex> face, std::vector<unsigned> indices);

public:
    Cube();
    void pre_draw() override;
};