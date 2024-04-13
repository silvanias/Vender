#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>

GLFWwindow *createWindow(GLint scr_width = 800, GLint scr_height = 600);
void glfwShutdown(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

#endif