#include <iostream>

#define GLFW_DLL
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/backends/imgui_impl_glfw.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb/stb_image.h"
#include "shader.h"
#include "camera/camera.h"
#include "material.h"
#include "models/objects/cube.h"
#include "models/objects/pyramid.h"
#include "models/lighting/light.h"

GLFWwindow *createWindow();
void setupGLFWCallbacks(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void initImGui(GLFWwindow *window);
unsigned int loadTexture(const char *path);

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
  auto clear_color = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);

  glEnable(GL_DEPTH_TEST);

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
    glBindVertexArray(VAOLight);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Normal cube rendering
    // ---------------------------------------------
    if (selectedMaterial < 2)
    {
      genericShader.use();
      genericShader.setVec3("viewPos", camera.cameraPos);

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
      texShader.setVec3("viewPos", camera.cameraPos);

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
    ImGui::Text("FPS = %f", ImGui::GetIO().Framerate);
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
    camera.processKeyboard(Direction::UP, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.processKeyboard(Direction::DOWN, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.processKeyboard(Direction::LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.processKeyboard(Direction::RIGHT, deltaTime);
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

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const *path)
{
  unsigned int textureID;
  glGenTextures(1, &textureID);

  int width;
  int height;
  int nrComponents;
  unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
  if (data)
  {
    GLenum format;
    if (nrComponents == 1)
      format = GL_RED;
    else if (nrComponents == 3)
      format = GL_RGB;
    else if (nrComponents == 4)
      format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
  }
  else
  {
    std::cout << "Texture failed to load at path: " << path << std::endl;
    stbi_image_free(data);
  }

  return textureID;
}