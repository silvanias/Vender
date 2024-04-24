#include "imgui_render.h"
#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/backends/imgui_impl_glfw.h"

void renderUI(const float framerate, Light &light, Material &material, int &selectedMaterial, int &selectedShape)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Controls");
    ImGui::Text("FPS = %f", framerate);
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
}
