#pragma once

#include <glad/glad.h>
#include "appdata/appdata.h"
#include "camera/camera.h"
#include "shaders/shader.h"
#include "assets/material.h"
#include "models/objects/shape.h"
#include "models/lighting/light.h"

void renderGenShapes(const std::array<std::unique_ptr<AbstractShape>, 5> &objects, const int &selectedShape, const Shader &shader, const Camera &camera, const Material &material, const std::array<glm::mat4, 3> &MVP, const Light &light);
void renderTexShapes(const std::array<std::unique_ptr<AbstractShape>, 5> &objects, const int &selectedShape, const Shader &shader, const Camera &camera, const Material &material, const std::array<glm::mat4, 3> &MVP, const Light &light, const unsigned int &diffuseMap, const unsigned int &specularMap);
void renderLights(const std::array<std::unique_ptr<AbstractShape>, 5> &objects, const Shader &shader, const std::array<glm::mat4, 3> &MVP, const Light &light);
void clearFrame(const ImVec4 &clear_color);
std::array<glm::mat4, 3> calculateMVP(const Camera &camera, float ratio, const glm::vec3 &pos = glm::vec3(0.0f, 0.0f, 0.0f), float scale = 1.0f);
void updateDeltaTime(AppData &appData);