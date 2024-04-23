#include <iostream>

#define GLFW_DLL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "stb/stb_image.h"

#include "appdata.h"
#include "input/input.h"
#include "gui/window/window.h"
#include "gui/imgui/imgui_lifecycle.h"
#include "camera/camera.h"
#include "shaders/shader.h"
#include "assets/texture.h"
#include "assets/material.h"
#include "models/objects/cube/cube.h"
#include "models/objects/pyramid/pyramid.h"
#include "models/lighting/light.h"

enum ShaderIdx
{
  generic,
  tex,
  light,
};

void renderLoop(GLFWwindow *window, const std::unique_ptr<AppData> &appData, const ImVec4 &clear_color, std::array<std::unique_ptr<Shader>, 3> &shaders, std::array<AbstractShape *, 5> &objects, Light &light, Material &material, unsigned int diffuseMap, unsigned int specularMap, int &selectedMaterial, int &selectedShape)
{

  // Object indices
  const int cubeNormIdx = 0;
  const int cubeTexIdx = 1;
  const int pyramidNormIdx = 2;
  const int pyramidTexIdx = 3;
  const int lightCubeIdx = 4;

  while (!glfwWindowShouldClose(window))
  {
    const Camera &camera = Camera::getInstance();
    auto currentFrame = static_cast<float>(glfwGetTime());
    appData->deltaTime = currentFrame - appData->lastFrame;
    appData->lastFrame = currentFrame;

    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    processInput(window);

    // Light rendering
    // -----------------------
    shaders[ShaderIdx::light]->use();
    shaders[ShaderIdx::light]->setVec3("lightColor", light.color);

    glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)appData->framebufferWidth / (float)appData->framebufferHeight, 0.1f, 100.0f);
    shaders[ShaderIdx::light]->setMat4("projection", projection);

    glm::mat4 view = camera.calculateView();
    shaders[ShaderIdx::light]->setMat4("view", view);

    auto model = glm::mat4(1.0f);
    model = glm::translate(model, light.pos);
    model = glm::scale(model, glm::vec3(0.2f));

    shaders[ShaderIdx::light]->setMat4("model", model);

    // Draw the light cube
    objects[lightCubeIdx]->render();

    // Normal cube rendering
    // ---------------------------------------------
    if (selectedMaterial < 2)
    {
      shaders[ShaderIdx::generic]->use();
      shaders[ShaderIdx::generic]->setVec3("viewPos", camera.cameraPos);

      shaders[ShaderIdx::generic]->setVec3("light.pos", light.pos);
      shaders[ShaderIdx::generic]->setVec3("light.ambient", light.ambient * light.color);
      shaders[ShaderIdx::generic]->setVec3("light.diffuse", light.diffuse * light.color);
      shaders[ShaderIdx::generic]->setVec3("light.specular", light.specular * light.color);

      shaders[ShaderIdx::generic]->setVec3("material.ambient", material.ambient);
      shaders[ShaderIdx::generic]->setVec3("material.diffuse", material.diffuse);
      shaders[ShaderIdx::generic]->setVec3("material.specular", material.specular);
      shaders[ShaderIdx::generic]->setFloat("material.shininess", material.shininess);

      shaders[ShaderIdx::generic]->setMat4("projection", projection);
      shaders[ShaderIdx::generic]->setMat4("view", view);
      model = glm::mat4(1.0f);
      shaders[ShaderIdx::generic]->setMat4("model", model);

      // TODO: Fix this whole ordeal
      if (selectedShape < 1)
      {
        objects[cubeNormIdx]->render();
      }
      else
      {
        objects[pyramidNormIdx]->render();
      }
    }

    // Textured cube rendering
    // ---------------------------------------------
    else if (selectedMaterial == 2)
    {
      shaders[ShaderIdx::tex]->use();
      shaders[ShaderIdx::tex]->setVec3("viewPos", camera.cameraPos);

      shaders[ShaderIdx::tex]->setVec3("light.pos", light.pos);
      shaders[ShaderIdx::tex]->setVec3("light.ambient", light.ambient * light.color);
      shaders[ShaderIdx::tex]->setVec3("light.diffuse", light.diffuse * light.color);
      shaders[ShaderIdx::tex]->setVec3("light.specular", light.specular * light.color);

      shaders[ShaderIdx::tex]->setVec3("material.specular", material.specular);
      shaders[ShaderIdx::tex]->setFloat("material.shininess", material.shininess);

      shaders[ShaderIdx::tex]->setMat4("projection", projection);
      shaders[ShaderIdx::tex]->setMat4("view", view);
      model = glm::mat4(1.0f);
      shaders[ShaderIdx::tex]->setMat4("model", model);

      // Bind diffuse map
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, diffuseMap);

      // Bind specular map
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, specularMap);

      if (selectedShape < 1)
      {
        objects[cubeTexIdx]->render();
      }
      else
      {
        objects[pyramidTexIdx]->render();
      }
    }

    // Render UI
    // ---------------------------------------------
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Controls");
    ImGui::Text("FPS = %f", appData->io.Framerate);
    if (ImGui::CollapsingHeader("Light"))
    {
      ImGui::SliderFloat3("Pos", glm::value_ptr(light.pos), -2.0f, 2.0f);
      ImGui::ColorPicker3("Color", glm::value_ptr(light.color));
    }

    if (ImGui::CollapsingHeader("Object"))
    {
      ImGui::RadioButton("Cube", &selectedShape, 0);
      if (selectedShape == 0)
      {
        ImGui::Indent();
        if (ImGui::RadioButton("Generic", &selectedMaterial, 0))
        {
          material = mat_generic;
        }
        else if (ImGui::RadioButton("Gold", &selectedMaterial, 1))
        {
          material = mat_gold;
        }
        ImGui::RadioButton("Container", &selectedMaterial, 2);
        ImGui::Unindent();
      }

      ImGui::RadioButton("Pyramid", &selectedShape, 1);
      // TODO: Seperate material and shape code

      ImGui::ColorPicker3("Ambient", glm::value_ptr(material.ambient));
      ImGui::ColorPicker3("Diffuse", glm::value_ptr(material.diffuse));
      ImGui::ColorPicker3("SpecularColor", glm::value_ptr(material.specular));
      ImGui::SliderFloat("Shininess", &material.shininess, 1.0f, 200.0f);
    }
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // glfw: swap buffers and poll IO events
    // ---------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

std::array<std::unique_ptr<Shader>, 3> loadShaders()
{
  std::array<std::unique_ptr<Shader>, 3> shaders = {
      std::make_unique<Shader>("../vender/shaders/vertex/obj_generic.vs", "../vender/shaders/fragment/obj_generic.fs"),
      std::make_unique<Shader>("../vender/shaders/vertex/obj_textured.vs", "../vender/shaders/fragment/obj_textured.fs"),
      std::make_unique<Shader>("../vender/shaders/vertex/obj_generic.vs", "../vender/shaders/fragment/point_light.fs")};
  return shaders;
}

void configureShaders(std::array<std::unique_ptr<Shader>, 3> &shaders)
{
  shaders[ShaderIdx::tex]->use();
  shaders[ShaderIdx::tex]->setInt("material.diffuse", 0);
  shaders[ShaderIdx::tex]->setInt("material.specular", 1);
}

int main()
{
  GLFWwindow *window = createWindow();
  if (window == nullptr)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  configWindow(window);
  if (!initializeGlAD())
  {
    return -1;
  }
  glEnable(GL_DEPTH_TEST);

  initImGui(window);

  const std::unique_ptr<AppData> appData = initAppData(window);
  glfwSetWindowUserPointer(window, appData.get());

  unsigned int diffuseMap = loadTexture("../assets/textures/container.png");
  unsigned int specularMap = loadTexture("../assets/textures/container_specular.png");
  auto shaders = loadShaders();
  configureShaders(shaders);

  CubeNorm cubeNorm;
  CubeTex cubeTex;
  PyramidNorm pyramidNorm;
  PyramidTex pyramidTex;
  CubeDefault lightCube;
  std::array<AbstractShape *, 5> objects = {
      &cubeNorm, &cubeTex, &pyramidNorm, &pyramidTex, &lightCube};

  Material material = mat_generic;

  Light light;

  light.pos = glm::vec3(1.0f, 0.17f, 1.6f);
  light.color = glm::vec3(1.0f, 1.0f, 1.0f);
  light.ambient = 0.2f;
  light.diffuse = 0.5f;
  light.specular = 1.0f;

  // ImGui configuration
  // --------------------
  int selectedMaterial = 0;
  int selectedShape = 0;

  auto clear_color = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
  renderLoop(window, appData, clear_color, shaders, objects, light, material, diffuseMap, specularMap, selectedMaterial, selectedShape);

  ImGuiShutdown();
  glfwShutdown(window);
  return 0;
}