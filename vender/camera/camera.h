#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum struct Direction
{
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera
{
public:
    float yaw = -90.0f;
    float pitch = 0.0f;
    float fov = 45.0f;
    float speed = 2.5f;
    float sensitivity = 0.1f;

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 calculateView() const;
    void processKeyboard(Direction direction, float deltaTime);
    void processMouse(float xoffset, float yoffset);
    void processZoom(float yoffset);
};