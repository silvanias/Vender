#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "window.h"
#include "appdata.h"
#include "input/input.h"

GLFWwindow *createWindow(GLint scr_width, GLint scr_height)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // safe on mac
    GLFWwindow *window = glfwCreateWindow(scr_width, scr_height, "vender", nullptr, nullptr);

    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    return window;
}

void configWindow(GLFWwindow *window)
{
    glfwMakeContextCurrent(window);
    setupGLFWCallbacks(window);
    glfwSwapInterval(1);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

bool initializeGlAD()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    return true;
}

void glfwShutdown(GLFWwindow *window)
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void setupGLFWCallbacks(GLFWwindow *window)
{
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetKeyCallback(window, keyCallback);
}

// glfw: window size changed, callback executes
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    auto appData = (AppData *)glfwGetWindowUserPointer(window);
    appData->framebufferWidth = width;
    appData->framebufferHeight = height;
    glViewport(0, 0, width, height);
}
