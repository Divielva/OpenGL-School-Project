#pragma once
#include <map>
#include <glm/glm.hpp>

#include "Camera.h"
#include "Shader.h"
#include "glfw/glfw3.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtc/type_ptr.hpp"

class InputProcessing
{
    glm::mat4 projection;
    std::map<int, void (*)()> keyboard_listeners;
    std::map<int, bool> should_repeat;
    std::map<int, bool> key_pressed;
    float zoom;
    float aspect;

public:
    InputProcessing();
    void change_aspect(float width, float height);
    void process_keyboard(GLFWwindow* window, double delta_time);
    void attach_keyboard_listener(int key, void (*event_handler)(), bool repeat);
    void remove_keyboard_listener(int key);
    void set_shader(const Shader* shader);
    glm::mat4 get_projection() const;
};

