#pragma once

#include <GLFW/glfw3.h>

void processInput(GLFWwindow *window);
void keyCallback(GLFWwindow *window, int key, int, int action, int);
void mouseCallback(GLFWwindow *, double xpos, double ypos);
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);