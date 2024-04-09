#include "ShaderStore.h"

#include <map>

std::map<unsigned, Shader*> shaders;
std::map<unsigned, std::string> shaderNames;

Shader* ShaderStore::add_shader(const std::string& name, const char* vertex_path, const char* fragment_path)
{
    const auto shader = new Shader(vertex_path, fragment_path);
    const auto id = shaders.size();
    shaders[id] = shader;
    shaderNames[id] = name;
    return shader;
}

Shader* ShaderStore::get_shader(const unsigned id)
{
    return shaders[id];
}

Shader* ShaderStore::get_shader(const std::string name)
{
    if (shaderNames.empty())
    {
        return nullptr;
    }
    for (const auto& shader : shaderNames)
    {
        if (shader.second == name)
        {
            return shaders[shader.first];
        }
    }
    return nullptr;
}

void ShaderStore::remove_all_shaders()
{
    for (const auto& shader : shaders)
    {
        delete shader.second;
    }
    shaders.clear();
}

void ShaderStore::remove_shader(const unsigned id)
{
    delete shaders[id];
    shaders.erase(id);
}

void ShaderStore::set_shader_params(void func(const Shader*))
{
    for (const auto& [id, shader] : shaders)
    {
        shader->set_params_callback(func);
    }
}
