#pragma once

#include "utils/render_utils.h"
#include "gui/window/window.h"
#include "gui/imgui/render/imgui_render.h"
#include "gui/imgui/lifecycle/imgui_lifecycle.h"
#include "input/input.h"
#include "camera/camera.h"
#include "shaders/shader.h"
#include "assets/material.h"
#include "models/objects/object_utils.h"
#include "models/lighting/light.h"

void renderLoop(GLFWwindow *window, AppData &appData,
                const std::array<std::unique_ptr<AbstractShape>, 5> &objects,
                const std::array<std::unique_ptr<Shader>, 3> &shaders,
                int &selectedShape,
                Material &material,
                int &selectedMaterial,
                Light &light,
                const ImVec4 &clear_color,
                unsigned int diffuseMap,
                unsigned int specularMap)
{
    while (!glfwWindowShouldClose(window))
    {
        const Camera &camera = Camera::getInstance();
        updateDeltaTime(appData);
        clearFrame(clear_color);
        processInput(window);

        auto MVP = calculateMVP(camera, (float)appData.framebufferWidth / (float)appData.framebufferHeight);
        renderLights(objects, *shaders[(size_t)ShaderIdx::light], MVP, light);
        if (selectedMaterial < 2)
        {
            renderGenShapes(objects, selectedShape, *shaders[(size_t)ShaderIdx::generic], camera, material, MVP, light);
        }
        else
        {
            renderTexShapes(objects, selectedShape, *shaders[(size_t)ShaderIdx::tex], camera, material, MVP, light, diffuseMap, specularMap);
        }

        renderUI(appData.io.Framerate, light, material, selectedMaterial, selectedShape);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
