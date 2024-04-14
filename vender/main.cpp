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
#include "models/objects/cube.h"
#include "models/objects/pyramid.h"
#include "models/lighting/light.h"

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

  std::unique_ptr<AppData> appData = initAppData(window);
  glfwSetWindowUserPointer(window, appData.get());

  auto clear_color = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);

  Shader genericShader("../vender/shaders/vertex/obj_generic.vs", "../vender/shaders/fragment/obj_generic.fs");
  Shader texShader("../vender/shaders/vertex/obj_textured.vs", "../vender/shaders/fragment/obj_textured.fs");
  Shader lightShader("../vender/shaders/vertex/obj_generic.vs", "../vender/shaders/fragment/point_light.fs");

  CubeNorm cubeNorm;
  CubeTex cubeTex;
  PyramidNorm pyramidNorm;
  PyramidTex pyramidTex;
  CubeDefault lightCube;
  auto [VBOCubeNorm, VAOCubeNorm] = cubeNorm.setupBuffers();
  auto [VBOCubeTex, VAOCubeTex] = cubeTex.setupBuffers();
  auto [VBOPyramidNorm, VAOPyramidNorm] = pyramidNorm.setupBuffers();
  auto [VBOPyramidTex, VAOPyramidTex] = pyramidTex.setupBuffers();
  auto [VBOLight, VAOLight] = lightCube.setupBuffers();

  Material material = mat_generic;
  unsigned int diffuseMap = loadTexture("../assets/textures/container.png");
  unsigned int specularMap = loadTexture("../assets/textures/container_specular.png");

  // Shader configuration
  // --------------------
  texShader.use();
  texShader.setInt("material.diffuse", 0);
  texShader.setInt("material.specular", 1);

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

  // Render loop
  // --------------------
  while (!glfwWindowShouldClose(window))
  {
    // auto appData = (AppData *)glfwGetWindowUserPointer(window);

    auto currentFrame = static_cast<float>(glfwGetTime());
    appData->deltaTime = currentFrame - appData->lastFrame;
    appData->lastFrame = currentFrame;

    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    processInput(window);

    // Light rendering
    // -----------------------
    lightShader.use();
    lightShader.setVec3("lightColor", light.color);

    glm::mat4 projection = glm::perspective(glm::radians(appData->camera->fov), (float)appData->framebufferWidth / (float)appData->framebufferHeight, 0.1f, 100.0f);
    lightShader.setMat4("projection", projection);

    glm::mat4 view = appData->camera->calculateView();
    lightShader.setMat4("view", view);

    auto model = glm::mat4(1.0f);
    model = glm::translate(model, light.pos);
    model = glm::scale(model, glm::vec3(0.2f));

    lightShader.setMat4("model", model);

    // Draw the light cube
    glBindVertexArray(VAOLight);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Normal cube rendering
    // ---------------------------------------------
    if (selectedMaterial < 2)
    {
      genericShader.use();
      genericShader.setVec3("viewPos", appData->camera->cameraPos);

      genericShader.setVec3("light.pos", light.pos);
      genericShader.setVec3("light.ambient", light.ambient * light.color);
      genericShader.setVec3("light.diffuse", light.diffuse * light.color);
      genericShader.setVec3("light.specular", light.specular * light.color);

      genericShader.setVec3("material.ambient", material.ambient);
      genericShader.setVec3("material.diffuse", material.diffuse);
      genericShader.setVec3("material.specular", material.specular);
      genericShader.setFloat("material.shininess", material.shininess);

      genericShader.setMat4("projection", projection);
      genericShader.setMat4("view", view);
      model = glm::mat4(1.0f);
      genericShader.setMat4("model", model);

      // TODO: Fix this whole ordeal
      if (selectedShape < 1)
      {
        // Draw the cube
        glBindVertexArray(VAOCubeNorm);
        glDrawArrays(GL_TRIANGLES, 0, 36);
      }
      else
      {
        // Temporary Pyramid Rendering
        glBindVertexArray(VAOPyramidNorm);
        glDrawArrays(GL_TRIANGLES, 0, 18);
      }
    }

    // Textured cube rendering
    // ---------------------------------------------
    else if (selectedMaterial == 2)
    {
      texShader.use();
      texShader.setVec3("viewPos", appData->camera->cameraPos);

      texShader.setVec3("light.pos", light.pos);
      texShader.setVec3("light.ambient", light.ambient * light.color);
      texShader.setVec3("light.diffuse", light.diffuse * light.color);
      texShader.setVec3("light.specular", light.specular * light.color);

      texShader.setVec3("material.specular", material.specular);
      texShader.setFloat("material.shininess", material.shininess);

      texShader.setMat4("projection", projection);
      texShader.setMat4("view", view);
      model = glm::mat4(1.0f);
      texShader.setMat4("model", model);

      // Bind diffuse map
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, diffuseMap);

      // Bind specular map
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, specularMap);

      if (selectedShape < 1)
      {
        glBindVertexArray(VAOCubeTex);
        glDrawArrays(GL_TRIANGLES, 0, 36);
      }
      else
      {
        // Temporary Pyramid Rendering
        glBindVertexArray(VAOPyramidTex);
        glDrawArrays(GL_TRIANGLES, 0, 24);
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

  glDeleteVertexArrays(1, &VAOCubeNorm);
  glDeleteBuffers(1, &VBOCubeNorm);
  glDeleteVertexArrays(1, &VAOCubeTex);
  glDeleteBuffers(1, &VBOCubeTex);
  glDeleteVertexArrays(1, &VAOPyramidNorm);
  glDeleteBuffers(1, &VBOPyramidNorm);
  glDeleteVertexArrays(1, &VAOPyramidTex);
  glDeleteBuffers(1, &VBOPyramidTex);
  glDeleteVertexArrays(1, &VAOLight);
  glDeleteBuffers(1, &VBOLight);

  ImGuiShutdown();
  glfwShutdown(window);
  return 0;
}
