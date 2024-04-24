#pragma once

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/backends/imgui_impl_glfw.h"

#include "appdata/appdata.h"
#include "input/input.h"
#include "gui/window/window.h"
#include "gui/imgui/imgui_lifecycle.h"
#include "camera/camera.h"
#include "shaders/shader.h"
#include "assets/material.h"
#include "models/objects/object_utils.h"
#include "models/lighting/light.h"

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
        auto currentFrame = static_cast<float>(glfwGetTime());
        appData->deltaTime = currentFrame - appData->lastFrame;
        appData->lastFrame = currentFrame;

        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        processInput(window);

        // Light rendering
        // -----------------------
        glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)appData->framebufferWidth / (float)appData->framebufferHeight, 0.1f, 100.0f);
        glm::mat4 view = camera.calculateView();
        auto model = glm::mat4(1.0f);
        model = glm::translate(model, light.pos);
        model = glm::scale(model, glm::vec3(0.2f));

        shaders[ShaderIdx::light]->use();
        shaders[ShaderIdx::light]->setVec3("lightColor", light.color);
        shaders[ShaderIdx::light]->setMat4("projection", projection);
        shaders[ShaderIdx::light]->setMat4("view", view);
        shaders[ShaderIdx::light]->setMat4("model", model);
        objects[ObjectIdx::lightCube]->render();

        // Normal cube rendering
        // ---------------------------------------------
        if (selectedMaterial < 2)
        {
            shaders[ShaderIdx::generic]->use();
            shaders[ShaderIdx::generic]->setVec3("viewPos", camera.cameraPos);

            shaders[ShaderIdx::generic]->setVec3("light.pos", light.pos);
            shaders[ShaderIdx::generic]->setVec3("light.ambient", light.ambient * light.color);
            shaders[ShaderIdx::generic]->setVec3("light.diffuse", light.diffuse * light.color);
            shaders[ShaderIdx::generic]->setVec3("light.specular", light.specular * light.color);

            shaders[ShaderIdx::generic]->setVec3("material.ambient", material.ambient);
            shaders[ShaderIdx::generic]->setVec3("material.diffuse", material.diffuse);
            shaders[ShaderIdx::generic]->setVec3("material.specular", material.specular);
            shaders[ShaderIdx::generic]->setFloat("material.shininess", material.shininess);

            shaders[ShaderIdx::generic]->setMat4("projection", projection);
            shaders[ShaderIdx::generic]->setMat4("view", view);
            model = glm::mat4(1.0f);
            shaders[ShaderIdx::generic]->setMat4("model", model);

            // TODO: Fix this whole ordeal
            if (selectedShape < 1)
            {
                objects[ObjectIdx::cubeNorm]->render();
            }
            else
            {
                objects[ObjectIdx::pyramidNorm]->render();
            }
        }

        // Textured cube rendering
        // ---------------------------------------------
        else if (selectedMaterial == 2)
        {
            shaders[ShaderIdx::tex]->use();
            shaders[ShaderIdx::tex]->setVec3("viewPos", camera.cameraPos);

            shaders[ShaderIdx::tex]->setVec3("light.pos", light.pos);
            shaders[ShaderIdx::tex]->setVec3("light.ambient", light.ambient * light.color);
            shaders[ShaderIdx::tex]->setVec3("light.diffuse", light.diffuse * light.color);
            shaders[ShaderIdx::tex]->setVec3("light.specular", light.specular * light.color);

            shaders[ShaderIdx::tex]->setVec3("material.specular", material.specular);
            shaders[ShaderIdx::tex]->setFloat("material.shininess", material.shininess);

            shaders[ShaderIdx::tex]->setMat4("projection", projection);
            shaders[ShaderIdx::tex]->setMat4("view", view);
            model = glm::mat4(1.0f);
            shaders[ShaderIdx::tex]->setMat4("model", model);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, diffuseMap);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, specularMap);

            if (selectedShape < 1)
            {
                objects[ObjectIdx::cubeTex]->render();
            }
            else
            {
                objects[ObjectIdx::pyramidTex]->render();
            }
        }

        // Render UI
        // ---------------------------------------------
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Controls");
        ImGui::Text("FPS = %f", appData->io.Framerate);
        if (ImGui::CollapsingHeader("Light"))
        {
            ImGui::SliderFloat3("Pos", glm::value_ptr(light.pos), -2.0f, 2.0f);
            ImGui::ColorPicker3("Color", glm::value_ptr(light.color));
        }

        if (ImGui::CollapsingHeader("Object"))
        {
            ImGui::Columns(2, nullptr, false);
            ImGui::RadioButton("Cube", &selectedShape, 0);
            ImGui::RadioButton("Pyramid", &selectedShape, 1);

            ImGui::NextColumn();
            if (ImGui::RadioButton("Generic", &selectedMaterial, 0))
            {
                material = Material();
            }
            else if (ImGui::RadioButton("Gold", &selectedMaterial, 1))
            {
                material = mat_gold;
            }
            ImGui::RadioButton("Container", &selectedMaterial, 2);
            ImGui::Columns(1);
            ImGui::Dummy(ImVec2(0.0f, 15.0f));

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
}
