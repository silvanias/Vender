#include "window.h"

GLFWwindow *createWindow(GLint scr_width, GLint scr_height)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // safe on mac

    return glfwCreateWindow(scr_width, scr_height, "vender", nullptr, nullptr);
}

void glfwShutdown(GLFWwindow *window)
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

// glfw: window size changed, callback executes
void framebuffer_size_callback(GLFWwindow *, int width, int height)
{
    glViewport(0, 0, width, height);
}
