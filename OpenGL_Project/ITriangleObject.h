#pragma once

#include "IObject.h"

class ITriangleObject : public IObject
{
public:
    ITriangleObject() : IObject()
    {
        this->draw_mode = GL_TRIANGLES;
    }
};