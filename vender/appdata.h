#ifndef APPDATA_H
#define APPDATA_H

#include <memory>
#include <GLFW/glfw3.h>
#include "camera/camera.h"
#include "imgui.h"

struct AppData
{
    const ImGuiIO &io;
    std::unique_ptr<Camera> camera;
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
        std::unique_ptr<Camera> _camera,
        int _fbw,
        int _fbh,
        float _lastX,
        float _lastY)
        : io(_io),
          camera(std::move(_camera)),
          framebufferWidth(_fbw),
          framebufferHeight(_fbh),
          lastX(_lastX),
          lastY(_lastY)
    {
    }
};

std::unique_ptr<AppData> initAppData(GLFWwindow *window);

#endif