#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>

GLFWwindow *createWindow(GLint scr_width = 800, GLint scr_height = 600);
void configWindow(GLFWwindow *window);
bool initializeGlAD();
void glfwShutdown(GLFWwindow *window);
void setupGLFWCallbacks(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

#endif