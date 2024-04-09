#pragma once

#include <iostream>

#include "ICollider.h"
#include "PointRender.h"
#include "glm/ext/quaternion_exponential.hpp"
#include "glm/gtx/rotate_vector.hpp"

struct OBB : ICollisionOld
{
    std::vector<glm::vec2> points;

    bool contains(ICollisionOld* collider) override
    {
        const auto colPoints = position == glm::vec2() ? points : get_points();
        for (auto point : collider->get_points())
        {
            auto i = 0;
            while (i + 2 < points.size())
            {
                auto a = colPoints[i];
                auto b = colPoints[i + 1] - a;
                auto c = colPoints[i + 2] - a;
                auto detA = (det(point, c) - det(a, c)) / det(b, c);
                auto detB = -(det(point, b) - det(a, b)) / det(b, c);
                if (detA > 0 && detB > 0 && detA + detB < 1)
                {
                    if (should_overlap)
                    {
                        return false;
                    }
                    return true;
                }
                ++i;
            }
        }

        return false;
    }

    void check_overlap(ICollisionOld* collider) override
    {
        const auto colPoints = get_points();
        for (auto point : collider->get_points())
        {
            auto i = 0;
            while (i + 2 < points.size())
            {
                auto a = colPoints[i];
                auto b = colPoints[i + 1] - a;
                auto c = colPoints[i + 2] - a;
                auto detA = (det(point, c) - det(a, c)) / det(b, c);
                auto detB = -(det(point, b) - det(a, b)) / det(b, c);
                if (detA > 0 && detB > 0 && detA + detB < 1)
                {
                    if (on_collision && should_overlap)
                    {
                        on_collision(parent, this, collider);
                        return;
                    }
                }
                ++i;
            }
        }
    }

    float det(const glm::vec2& a, const glm::vec2& b)
    {
        return a.x * b.y - a.y * b.x;
    }

    glm::vec2 cross(const glm::vec2& a, const glm::vec2& b)
    {
        return glm::vec2(a.x * b.y, a.y * b.x);
    }

    glm::vec2 get_center() const override
    {
        auto pos = glm::vec2(0);
        for (const auto point : points)
        {
            pos += point;
        }

        return pos / static_cast<float>(points.size());
    }

    glm::vec3 rebound(const glm::vec3 point, const glm::vec3 velocity) const override
    {
        return point;
    }

    void draw_bounds() override
    {
        if (!draw_object)
            draw_object = new PointRender();
        if (draw_object)
        {
            dynamic_cast<PointRender*>(draw_object)->clear_points();
            std::vector<glm::vec2> points;
            points.reserve(this->points.size());
            for (auto point : this->points)
            {
                points.push_back(rotate(point, glm::radians(angle)) + position);
            }
            dynamic_cast<PointRender*>(draw_object)->add_points(points);
            draw_object->draw();
        }
    }

    std::vector<glm::vec2> get_points() override
    {
        std::vector<glm::vec2> actualPoints;
        actualPoints.reserve(points.size());
        for (auto point : points)
        {
            actualPoints.push_back(rotate(point, glm::radians(angle)) + position);
        }
        return actualPoints;
    }
};