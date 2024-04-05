#include <iostream>

#define GLFW_DLL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLUT/glut.h>

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/backends/imgui_impl_glfw.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"
#include "shader.h"
#include "camera.h"
#include "material.h"
#include "models/cube_model.h"

GLFWwindow *createWindow();
void setupGLFWCallbacks(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void initImGui(GLFWwindow *window);

GLint SCR_WIDTH = 800;
GLint SCR_HEIGHT = 600;

Camera camera;

float lastX = SCR_WIDTH / 2;
float lastY = SCR_HEIGHT / 2;

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f;

bool firstMouse = true;

bool DEBUG_MODE = false;

int main()
{
  GLFWwindow *window = createWindow();
  if (window == nullptr)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  setupGLFWCallbacks(window);
  glfwSwapInterval(1);

  // Capture mouse
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  initImGui(window);

  glEnable(GL_DEPTH_TEST);

  Shader cubeShader("../shaders/shader.vs", "../shaders/cube_generic.fs");
  Shader lightShader("../shaders/shader.vs", "../shaders/light.fs");
  CubeDefault cube;
  CubeDefault lightCube;
  auto [VBO, cubeVAO] = cube.setupBuffers();
  auto [VBOL, lightVAO] = lightCube.setupBuffers(VBO);

  auto clear_color = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);

  // Refactor this
  // ---------------------------------------------

  struct Light
  {
    glm::vec3 pos;
    glm::vec3 color;
    float ambient;
    float diffuse;
    float specular;
  };

  Light light;

  light.pos = glm::vec3(1.0f, 0.17f, 1.6f);
  light.color = glm::vec3(1.0f, 1.0f, 1.0f);
  light.ambient = 0.2f;
  light.diffuse = 0.5f;
  light.specular = 1.0f;

  Material material = mat_generic;
  int selectedMaterial = 0;
  // Render loop
  while (!glfwWindowShouldClose(window))
  {
    auto currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    processInput(window);

    // Light rendering
    // -----------------------

    lightShader.use();
    lightShader.setVec3("lightColor", light.color);

    glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    lightShader.setMat4("projection", projection);

    glm::mat4 view = camera.calculateView();
    lightShader.setMat4("view", view);

    auto model = glm::mat4(1.0f);
    model = glm::translate(model, light.pos);
    model = glm::scale(model, glm::vec3(0.2f));

    lightShader.setMat4("model", model);

    // Draw the light cube
    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Cube rendering
    // ---------------------------------------------
    cubeShader.use();
    cubeShader.setVec3("viewPos", camera.cameraPos);

    cubeShader.setVec3("light.pos", light.pos);
    cubeShader.setVec3("light.ambient", light.ambient * light.color);
    cubeShader.setVec3("light.diffuse", light.diffuse * light.color);
    cubeShader.setVec3("light.specular", light.specular * light.color);

    cubeShader.setVec3("material.color", material.color);
    cubeShader.setVec3("material.ambient", material.ambient * material.color);
    cubeShader.setVec3("material.diffuse", material.diffuse * material.color);
    cubeShader.setVec3("material.specular", material.specular);
    cubeShader.setFloat("material.shininess", material.shininess);

    cubeShader.setMat4("projection", projection);
    cubeShader.setMat4("view", view);
    model = glm::mat4(1.0f);
    cubeShader.setMat4("model", model);

    // Draw the cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Render UI
    // ---------------------------------------------
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Controls");
    ImGui::Text("FPS = %f", ImGui::GetIO().Framerate);
    if (ImGui::CollapsingHeader("Light"))
    {
      ImGui::SliderFloat3("Pos", glm::value_ptr(light.pos), -2.0f, 2.0f);
      ImGui::ColorPicker3("Color", glm::value_ptr(light.color));
    }

    if (ImGui::CollapsingHeader("Object"))
    {
      if (ImGui::RadioButton("Generic", &selectedMaterial, 0))
      {
        material = mat_generic;
      }
      else if (ImGui::RadioButton("Gold", &selectedMaterial, 1))
      {
        material = mat_gold;
      }

      ImGui::ColorPicker3("MaterialColor", glm::value_ptr(material.color));
      ImGui::SliderFloat("Ambient", &material.ambient, 0.0f, 1.0f);
      ImGui::SliderFloat("Diffuse", &material.diffuse, 0.0f, 1.0f);
      ImGui::SliderFloat("Shininess", &material.shininess, 1.0f, 200.0f);
      ImGui::ColorPicker3("SpecularColor", glm::value_ptr(material.specular));
    }
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // glfw: swap buffers and poll IO events
    // ---------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteVertexArrays(1, &lightVAO);
  glDeleteBuffers(1, &VBO);

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.processKeyboard(UP, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.processKeyboard(DOWN, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.processKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.processKeyboard(RIGHT, deltaTime);
}

void key_callback(GLFWwindow *window, int key, int, int action, int)
{
  if (key == GLFW_KEY_M && action == GLFW_PRESS)
  {
    if (DEBUG_MODE)
    {
      DEBUG_MODE = false;
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    else
    {
      DEBUG_MODE = true;
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
  }
}

// glfw: window size changed, callback executes
void framebuffer_size_callback(GLFWwindow *, int width, int height)
{
  glViewport(0, 0, width, height);
  SCR_HEIGHT = height;
  SCR_WIDTH = width;
}

void mouse_callback(GLFWwindow *, double xpos, double ypos)
{

  if (const ImGuiIO &io = ImGui::GetIO(); io.WantCaptureMouse || DEBUG_MODE)
  {
    firstMouse = true;
    return;
  }

  if (firstMouse)
  {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;
  lastX = xpos;
  lastY = ypos;

  camera.processMouse(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
  if (const ImGuiIO &io = ImGui::GetIO(); io.WantCaptureMouse || DEBUG_MODE)
  {
    return;
  }
  camera.processZoom(yoffset);
}

void initImGui(GLFWwindow *window)
{
  const char *glsl_version = "#version 150";
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

  ImGui::StyleColorsDark();
  ImGui::SetNextWindowCollapsed(true);

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true); // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
  ImGui_ImplOpenGL3_Init(glsl_version);
}

GLFWwindow *createWindow()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // safe on mac

  return glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "vender", nullptr, nullptr);
}

void setupGLFWCallbacks(GLFWwindow *window)
{
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetKeyCallback(window, key_callback);
}