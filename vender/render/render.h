#pragma once

#include <glad/glad.h>

#include "appdata/appdata.h"
#include "gui/window/window.h"
#include "gui/imgui/render/imgui_render.h"
#include "gui/imgui/lifecycle/imgui_lifecycle.h"
#include "input/input.h"
#include "camera/camera.h"
#include "shaders/shader.h"
#include "assets/material.h"
#include "models/objects/object_utils.h"
#include "models/lighting/light.h"

void bindTextures(unsigned int diffuseMap, unsigned int specularMap)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);
}

std::array<glm::mat4, 3> calculateMVP(const Camera &camera, float ratio, const glm::vec3 &pos = glm::vec3(0.0f, 0.0f, 0.0f), float scale = 1.0f)
{
    glm::mat4 projection = glm::perspective(glm::radians(camera.fov), ratio, 0.1f, 100.0f);
    glm::mat4 view = camera.calculateView();
    auto model = glm::mat4(1.0f);
    model = glm::translate(model, pos);
    model = glm::scale(model, glm::vec3(scale));
    return std::array<glm::mat4, 3>{
        model,
        view,
        projection};
}

void setShaderMVP(const std::unique_ptr<Shader> &shader, const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection)
{
    shader->setMat4("model", model);
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);
}

void renderGenShapes(const std::array<std::unique_ptr<AbstractShape>, 5> &objects, const int &selectedShape, const std::unique_ptr<Shader> &shader, const Camera &camera, const Material &material, struct std::array<glm::mat4, 3> &MVP, const Light &light)
{
    shader->use();
    setShaderLighting(shader, light);
    shader->setVec3("viewPos", camera.cameraPos);
    setShaderMVP(shader, MVP[0], MVP[1], MVP[2]);

    shader->setVec3("material.ambient", material.ambient);
    shader->setVec3("material.diffuse", material.diffuse);
    shader->setVec3("material.specular", material.specular);
    shader->setFloat("material.shininess", material.shininess);

    if (selectedShape < 1)
    {
        objects[ObjectIdx::cubeNorm]->render();
    }
    else
    {
        objects[ObjectIdx::pyramidNorm]->render();
    }
}

void renderTexShapes(const std::array<std::unique_ptr<AbstractShape>, 5> &objects, const int &selectedShape, const std::unique_ptr<Shader> &shader, const Camera &camera, const Material &material, struct std::array<glm::mat4, 3> &MVP, const Light &light, const unsigned int &diffuseMap, const unsigned int &specularMap)
{
    shader->use();
    setShaderLighting(shader, light);
    shader->setVec3("viewPos", camera.cameraPos);
    setShaderMVP(shader, MVP[0], MVP[1], MVP[2]);

    shader->setVec3("material.specular", material.specular);
    shader->setFloat("material.shininess", material.shininess);

    bindTextures(diffuseMap, specularMap);

    if (selectedShape < 1)
    {
        objects[ObjectIdx::cubeTex]->render();
    }
    else
    {
        objects[ObjectIdx::pyramidTex]->render();
    }
}

void renderLights(const std::array<std::unique_ptr<AbstractShape>, 5> &objects, const std::unique_ptr<Shader> &shader, struct std::array<glm::mat4, 3> &MVP, const Light &light)
{
    shader->use();
    auto model = glm::mat4(1.0f);
    model = glm::translate(model, light.pos);
    model = glm::scale(model, glm::vec3(0.2f));
    setShaderMVP(shader, model, MVP[1], MVP[2]);
    shader->setVec3("lightColor", light.color);

    objects[ObjectIdx::lightCube]->render();
}

void updateDeltaTime(const std::unique_ptr<AppData> &appData)
{
    auto currentFrame = static_cast<float>(glfwGetTime());
    appData->deltaTime = currentFrame - appData->lastFrame;
    appData->lastFrame = currentFrame;
}

void clearFrame(const ImVec4 &clear_color)
{
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderLoop(GLFWwindow *window, const std::unique_ptr<AppData> &appData,
                const ImVec4 &clear_color,
                std::array<std::unique_ptr<Shader>, 3> &shaders,
                std::array<std::unique_ptr<AbstractShape>, 5> &objects,
                Light &light,
                Material &material,
                unsigned int diffuseMap,
                unsigned int specularMap,
                int &selectedMaterial,
                int &selectedShape)
{
    while (!glfwWindowShouldClose(window))
    {
        const Camera &camera = Camera::getInstance();
        updateDeltaTime(appData);
        clearFrame(clear_color);
        processInput(window);

        auto MVP = calculateMVP(camera, (float)appData->framebufferWidth / (float)appData->framebufferHeight);
        renderLights(objects, shaders[ShaderIdx::light], MVP, light);
        if (selectedMaterial < 2)
        {
            renderGenShapes(objects, selectedShape, shaders[ShaderIdx::generic], camera, material, MVP, light);
        }
        else
        {
            renderTexShapes(objects, selectedShape, shaders[ShaderIdx::tex], camera, material, MVP, light, diffuseMap, specularMap);
        }

        renderUI(appData->io.Framerate, light, material, selectedMaterial, selectedShape);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
