#include "camera/camera.h"
#include "imgui.h"

struct AppData
{
    const ImGuiIO &io;
    Camera *camera;
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
        Camera *_camera,
        int _fbw,
        int _fbh,
        float _lastX,
        float _lastY)
        : io(_io),
          camera(_camera),
          framebufferWidth(_fbw),
          framebufferHeight(_fbh),
          lastX(_lastX),
          lastY(_lastY)
    {
    }
};