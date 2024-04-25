#include "camera.h"

glm::mat4 Camera::calculateView() const
{
    return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::processKeyboard(Direction direction, float deltaTime)
{
    using enum Direction;
    float cameraSpeed = speed * deltaTime;
    if (direction == UP)
        cameraPos += cameraSpeed * cameraFront;
    if (direction == DOWN)
        cameraPos -= cameraSpeed * cameraFront;
    if (direction == LEFT)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (direction == RIGHT)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void Camera::processMouse(float xoffset, float yoffset)
{

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

void Camera::processZoom(float yoffset)
{
    fov -= yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 90.0f)
        fov = 90.0f;
}
