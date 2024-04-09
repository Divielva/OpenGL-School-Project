#include "Window.h"

#include <iostream>

#include "ShaderStore.h"
#include "TimeManager.h"
#include <filesystem>

Window window;

int main()
{
    auto path = std::filesystem::current_path();
    std::cout << path << std::endl;
    // check if path ends with Debug if not append Debug and set as current path
    //if (path.string().find("Debug") == std::string::npos)
    //{
    //    path = path / "Debug";
    //    std::filesystem::current_path(path);
    //}

    auto winRet = window.init();
    if (winRet != 0)
    {
        return winRet;
    }
    ShaderStore::add_shader("default", "shaders/default.vs", "shaders/default.fs");
    //ShaderStore::add_shader("uvVisual", "shaders/default.vs", "shaders/texCoordVisual.fs");
    ShaderStore::add_shader("noLight", "shaders/default.vs", "shaders/noLight.fs");
    ShaderStore::add_shader("shadowMap", "shaders/shadowMap.vs", "shaders/shadowMap.fs");

    window.create_objects();

    while (!window.should_close())
    {
        window.update();
    }

    return 0;
}

