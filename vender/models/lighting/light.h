#pragma once

#include <glm/glm.hpp>

struct Light
{
    glm::vec3 pos = glm::vec3(1.0f, 0.17f, 1.6f);
    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
    float ambient = 0.2f;
    float diffuse = 0.5f;
    float specular = 1.0f;
};