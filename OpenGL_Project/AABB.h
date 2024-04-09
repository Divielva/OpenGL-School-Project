#pragma once

#include <glm/glm.hpp>
#include <memory>

struct AABB
{
    glm::vec3 center;
    glm::vec3 extent;

    AABB() : center(glm::vec3(0)), extent(glm::vec3(0)) {}

    AABB(const glm::vec3& center, const glm::vec3& extent) : center(center), extent(extent) {}

    bool intersect(const AABB& other, glm::vec3* mtv) const
    {
        glm::vec3 diff = other.center - center;
        glm::vec3 sumExtent = extent + other.extent;

        for (int i = 0; i < 3; i++)
        {
            if (abs(diff[i]) > sumExtent[i])
            {
                return false;
            }
        }

        if (mtv)
        {
            glm::vec3 mtvValues;
            for (int i = 0; i < 3; ++i)
                mtvValues[i] = sumExtent[i] - abs(diff[i]);

            float minMTV = FLT_MAX;
            int minAxis = -1;
            for (int i = 0; i < 3; ++i)
            {
                if (mtvValues[i] < minMTV && mtvValues[i] > 0)
                {
                    minMTV = mtvValues[i];
                    minAxis = i;
                }
            }

            if (minAxis != -1)
            {
                glm::vec3 direction = glm::vec3(0);
                direction[minAxis] = diff[minAxis] > 0.f ? 1.f : -1.f;
                *mtv = direction * minMTV;
            }
        }

        return true;
    }
};