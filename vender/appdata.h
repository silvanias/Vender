#pragma once

#include <memory>
#include <GLFW/glfw3.h>
#include "imgui.h"

struct AppData
{
    const ImGuiIO &io;
    int framebufferWidth;
    int framebufferHeight;
    float lastX;
    float lastY;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    bool firstMouse = true;
    bool debug_mode = false;
    AppData(
        const ImGuiIO &_io,
        int _fbw,
        int _fbh,
        float _lastX,
        float _lastY)
        : io(_io),
          framebufferWidth(_fbw),
          framebufferHeight(_fbh),
          lastX(_lastX),
          lastY(_lastY)
    {
    }
};

std::unique_ptr<AppData> initAppData(GLFWwindow *window);