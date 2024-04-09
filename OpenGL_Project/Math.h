#pragma once
#include <vector>

#include "Mesh.h"
#include "glm/fwd.hpp"

#ifdef M_PI
#undef M_PI
#endif
#define M_PI 3.1415926535897932384626433832795

#ifndef LERP
#define LERP

template <typename T>
T lerp(const T& a, const T& b, const float t)
{
    return a + (b - a) * t;
}

template <>
Vertex lerp(const Vertex& a, const Vertex& b, float t);
#endif

#ifndef EULER_LERP
#define EULER_LERP
glm::vec3 euler_lerp(const glm::vec3& a, const glm::vec3& b, float t);
#endif

#ifndef CLAMP
#define CLAMP

template <typename T, typename U>
T clamp(const T& value, const U& min, const U& max)
{
    if (value < min)
        return min;
    if (value > max)
        return max;
    return value;
}
#endif

#ifndef BEZIER
#define BEZIER

template <typename T>
struct Bezier
{
    T p0, p1, p2, p3;

    Bezier() : p0(T()), p1(T()), p2(T()), p3(T())
    {
    }

    Bezier(T p0, T p1, T p2, T p3) : p0(p0), p1(p1), p2(p2), p3(p3)
    {
    }

    T operator()(const float t)
    {
        const auto u = 1 - t;
        const auto tp0 = powf(u, 3) * p0;
        const auto tp1 = 3 * powf(u, 2) * t * p1;
        const auto tp2 = 3 * u * powf(t, 2) * p2;
        const auto tp3 = powf(t, 3) * p3;
        return tp0 + tp1 + tp2 + tp3;
    }
};
#endif

#ifndef BSPLINE
// ReSharper disable once IdentifierTypo
#define BSPLINE

template <typename T>
struct BSpline
{
    std::vector<std::tuple<float, Bezier<T>>> points;

    void add_point(float t, Bezier<T> curve)
    {
        points.push_back(std::tuple<float, Bezier<T>>(t, curve));
    }

    T operator()(float t)
    {
        if (points.size() == 0)
            return T();
        if (points.size() == 1)
            return std::get<1>(points[0])(t);
        auto it = points.begin();
        auto prev = *it;
        auto cur = *++it;
        auto next = ++it;
        while (t >= std::get<0>(cur) && next != points.end())
        {
            prev = cur;
            cur = *next;
            next = ++it;
        }
        if (std::get<0>(prev) >= 1)
            return std::get<1>(prev)(1);
        auto u = (t - std::get<0>(prev)) / (std::get<0>(cur) - std::get<0>(prev));
        return std::get<1>(prev)(u);
    }
};
#endif

#ifndef MAT_MATH_H
#define MAT_MATH_H
glm::mat4 rot_z_mat(float angle);

glm::mat4 rot_y_mat(float angle);

glm::mat4 rot_x_mat(float angle);

glm::mat4 scale_mat(float x, float y, float z);

glm::mat4 translate_mat(float x, float y, float z);

glm::mat4 combine(const glm::mat4* mats...);
#endif