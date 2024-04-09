#pragma once

#include "IObject.h"

class Line : public IObject
{
	float width;

public:
	Line()
	{
		this->albedo = hsl(0,0,0);
		this->width = 1.0f;	
		draw_mode = GL_LINE_STRIP;	
	}
    Line(glm::vec3 start, glm::vec3 end, hsl color, float width = 1.0f) : Line()
    {
        vertices = { start, end };
        indices = { 0, 1 };
        this->albedo = color;
        this->width = width;
    }
    Line(vector<glm::vec3> points, hsl color, float width = 1.0f) : Line()
    {
        vertices.reserve(points.size());
        indices.reserve(points.size());
        for (int i = 0; i < points.size(); i++)
        {
            vertices.push_back(points[i]);
            indices.push_back(i);
        }
        this->albedo = color;
        this->width = width;
    }

    void pre_draw() override
    {
        glLineWidth(width);
    }

    void post_draw() override
    {
        glLineWidth(1.0f);
    }
};