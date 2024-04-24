#include "appdata.h"

std::unique_ptr<AppData> initAppData(GLFWwindow *window)
{
    const ImGuiIO &io = ImGui::GetIO();
    int framebufferWidth;
    int framebufferHeight;
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
    auto lastX = (float)framebufferWidth / 2;
    auto lastY = (float)framebufferHeight / 2;

    auto appDataPtr = std::make_unique<AppData>(io, framebufferWidth, framebufferHeight, lastX, lastY);
    return appDataPtr;
}
