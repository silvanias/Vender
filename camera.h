#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Direction
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

    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;

    Camera(
        glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f))
        : cameraPos(cameraPos), cameraFront(cameraFront), cameraUp(cameraUp)
    {
    }

    glm::mat4 calculateView() const
    {
        return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }

    void processKeyboard(Direction direction, float deltaTime)
    {
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

    void processMouse(float xoffset, float yoffset)
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

    void processZoom(float yoffset)
    {
        fov -= yoffset;
        if (fov < 1.0f)
            fov = 1.0f;
        if (fov > 90.0f)
            fov = 90.0f;
    }
};

#endif