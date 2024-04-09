#pragma once
#include <glm/vec3.hpp>
#include "HSL.h"
#include "Shader.h"

struct Light
{
    hsl ambient;
    hsl diffuse;
    glm::vec3 specular;

    virtual void set_shader(const Shader* shader)
    {
        shader->set_vec3(get_name() + ".light.ambient", ambient.get_rgb_vec3());
        shader->set_vec3(get_name() + ".light.diffuse", diffuse.get_rgb_vec3());
        shader->set_vec3(get_name() + ".light.specular", specular);
    }

    virtual std::string get_name()
    {
        return "light";
    }
};

struct DirectionalLight : public Light
{
    glm::vec3 direction;

    void set_shader(const Shader* shader) override
    {
        Light::set_shader(shader);
        shader->set_vec3(get_name() + ".direction", direction);
    }

    std::string get_name() override
    {
        return "dirLight";
    }
};

struct PointLight : public Light
{
    int index;
    glm::vec3 position;
    float constant;
    float linear;
    float quadratic;

    void set_shader(const Shader* shader) override
    {
        Light::set_shader(shader);
        shader->set_vec3(get_name() + ".position", position);
        shader->set_float(get_name() + ".constant", constant);
        shader->set_float(get_name() + ".linear", linear);
        shader->set_float(get_name() + ".quadratic", quadratic);
    }

    std::string get_name() override
    {
        return "pointLights[" + std::to_string(index) + "]";
    }
};

struct SpotLight : public PointLight
{
    glm::vec3 direction;
    float cutOff;
    float outerCutOff;

    void set_shader(const Shader* shader) override
    {
        PointLight::set_shader(shader);
        shader->set_vec3(get_name() + ".direction", direction);
        shader->set_float(get_name() + ".cutOff", cutOff);
        shader->set_float(get_name() + ".outerCutOff", outerCutOff);
    }

    std::string get_name() override
    {
        return "spotLight";
    }
};