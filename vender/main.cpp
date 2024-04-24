#define GLFW_DLL

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "appdata/appdata.h"
#include "render.h"
#include "assets/texture.h"
#include "assets/material.h"
#include "models/objects/object_utils.h"
#include "models/lighting/light.h"

int main()
{
  GLFWwindow *window = createWindow();
  configWindow(window);
  initializeGlAD();
  glEnable(GL_DEPTH_TEST);
  initImGui(window);
  const std::unique_ptr<AppData> appData = initAppData(window);
  glfwSetWindowUserPointer(window, appData.get());

  unsigned int diffuseMap = loadTexture("../assets/textures/container.png");
  unsigned int specularMap = loadTexture("../assets/textures/container_specular.png");
  auto shaders = loadShaders();
  configureShaders(shaders);
  auto objects = createObjects();

  Material material;
  Light light;
  int selectedMaterial = 0;
  int selectedShape = 0;
  auto clear_color = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);

  renderLoop(window,
             appData,
             clear_color,
             shaders,
             objects,
             light,
             material,
             diffuseMap,
             specularMap,
             selectedMaterial,
             selectedShape);

  ImGuiShutdown();
  glfwShutdown(window);
  return 0;
}
