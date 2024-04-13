#include "input.h"
#include "../camera/camera.h"
#include "../appdata.h"

void processInput(GLFWwindow *window)
{
    auto appData = (AppData *)(glfwGetWindowUserPointer(window));
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        appData->camera.processKeyboard(Direction::UP, appData->deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        appData->camera.processKeyboard(Direction::DOWN, appData->deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        appData->camera.processKeyboard(Direction::LEFT, appData->deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        appData->camera.processKeyboard(Direction::RIGHT, appData->deltaTime);
}

void keyCallback(GLFWwindow *window, int key, int, int action, int)
{
    auto appData = (AppData *)(glfwGetWindowUserPointer(window));
    if (key == GLFW_KEY_M && action == GLFW_PRESS)
    {
        if (appData->debug_mode)
        {
            appData->debug_mode = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else
        {
            appData->debug_mode = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos)
{
    auto appData = (AppData *)(glfwGetWindowUserPointer(window));
    if (appData->io.WantCaptureMouse || appData->debug_mode)
    {
        appData->firstMouse = true;
        return;
    }

    if (appData->firstMouse)
    {
        appData->lastX = xpos;
        appData->lastY = ypos;
        appData->firstMouse = false;
    }

    float xoffset = xpos - appData->lastX;
    float yoffset = appData->lastY - ypos;
    appData->lastX = xpos;
    appData->lastY = ypos;

    appData->camera.processMouse(xoffset, yoffset);
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    auto appData = (AppData *)(glfwGetWindowUserPointer(window));
    if (appData->io.WantCaptureMouse || appData->debug_mode)
    {
        return;
    }
    appData->camera.processZoom(yoffset);
}
