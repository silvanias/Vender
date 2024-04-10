#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>

struct Material
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

const Material mat_generic = {
    glm::vec3(1.0f, 0.5f, 0.5f), // ambient
    glm::vec3(1.0f, 0.5f, 0.5f), // diffuse
    glm::vec3(0.5f, 0.5f, 0.5f), // specular
    64.0f                        // shininess
};

// Metal approximation
const Material mat_gold = {
    glm::vec3(1.0f, 0.84f, 0.0f), // ambient
    glm::vec3(1.0f, 0.84f, 0.0f), // diffuse
    glm::vec3(0.5f, 0.5f, 0.5f),  // specular
    168.0f                        // shininess
};

#endif