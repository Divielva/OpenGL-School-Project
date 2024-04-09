#include "Math.h"

#include <cstdarg>

#include "Mesh.h"
#include "glm/gtc/matrix_transform.hpp"

glm::mat4 rot_z_mat(float angle)
{
    angle = glm::radians(angle);
    return {
        glm::vec4(cos(angle), -sin(angle), 0, 0),
        glm::vec4(sin(angle), cos(angle), 0, 0),
        glm::vec4(0, 0, 1, 0),
        glm::vec4(0, 0, 0, 1) };
}

glm::mat4 rot_y_mat(float angle)
{
    angle = glm::radians(angle);
    return {
        glm::vec4(cos(angle), 0, sin(angle), 0),
        glm::vec4(0, 1, 0, 0),
        glm::vec4(-sin(angle), 0, cos(angle), 0),
        glm::vec4(0, 0, 0, 1) };
}

glm::mat4 rot_x_mat(float angle)
{
    angle = glm::radians(angle);
    return {
        glm::vec4(1, 0, 0, 0),
        glm::vec4(0, cos(angle), -sin(angle), 0),
        glm::vec4(0, sin(angle), cos(angle), 0),
        glm::vec4(0, 0, 0, 1) };
}

glm::mat4 scale_mat(float x, float y, float z)
{
    return {
        glm::vec4(x, 0, 0, 0),
        glm::vec4(0, y, 0, 0),
        glm::vec4(0, 0, z, 0),
        glm::vec4(0, 0, 0, 1) };
}

glm::mat4 translate_mat(float x, float y, float z)
{
    return {
        glm::vec4(1, 0, 0, 0),
        glm::vec4(0, 1, 0, 0),
        glm::vec4(0, 0, 1, 0),
        glm::vec4(x, y, z, 1) };
}

glm::mat4 combine(const glm::mat4* mats...)
{
    auto result = glm::mat4(1);
    va_list args;
    va_start(args, mats);
    while (mats != nullptr)
    {
        result *= *mats;
        mats = va_arg(args, glm::mat4*);
    }
    return result;
}

float lerp_shortest(float a, float b, float t)
{
    if (b - a > 180)
        a += 360;
    if (b - a < -180)
        a -= 360;
    return a + t * (b - a);
}

glm::vec3 euler_lerp(const glm::vec3& a, const glm::vec3& b, const float t)
{
    return {
        lerp_shortest(a.x, b.x, t),
        lerp_shortest(a.y, b.y, t),
        lerp_shortest(a.z, b.z, t) };
}

template <>
Vertex lerp(const Vertex& a, const Vertex& b, const float t)
{
    return Vertex(
        lerp(a.position, b.position, t),
        lerp(a.normal, b.normal, t),
        lerp(a.texture_coord, b.texture_coord, t));
}