#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

struct Light
{
    glm::vec3 pos;
    glm::vec3 color;
    float ambient;
    float diffuse;
    float specular;
};

#endif