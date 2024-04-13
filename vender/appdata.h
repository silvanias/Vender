#include "camera/camera.h"
#include "imgui.h"

struct AppData
{
    const ImGuiIO &io;
    Camera camera;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float lastX;
    float lastY;
    bool firstMouse = true;
    bool debug_mode = false;
};