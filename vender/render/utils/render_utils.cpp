#include "render_utils.h"
#include "models/objects/object_utils.h"

void renderGenShapes(const std::array<std::unique_ptr<AbstractShape>, 5> &objects, const int &selectedShape, const std::unique_ptr<Shader> &shader, const Camera &camera, const Material &material, const std::array<glm::mat4, 3> &MVP, const Light &light)
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

void renderTexShapes(const std::array<std::unique_ptr<AbstractShape>, 5> &objects, const int &selectedShape, const std::unique_ptr<Shader> &shader, const Camera &camera, const Material &material, const std::array<glm::mat4, 3> &MVP, const Light &light, const unsigned int &diffuseMap, const unsigned int &specularMap)
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

void renderLights(const std::array<std::unique_ptr<AbstractShape>, 5> &objects, const std::unique_ptr<Shader> &shader, const std::array<glm::mat4, 3> &MVP, const Light &light)
{
    shader->use();
    auto model = glm::mat4(1.0f);
    model = glm::translate(model, light.pos);
    model = glm::scale(model, glm::vec3(0.2f));
    setShaderMVP(shader, model, MVP[1], MVP[2]);
    shader->setVec3("lightColor", light.color);

    objects[ObjectIdx::lightCube]->render();
}

void clearFrame(const ImVec4 &clear_color)
{
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

std::array<glm::mat4, 3> calculateMVP(const Camera &camera, float ratio, const glm::vec3 &pos, float scale)
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

void updateDeltaTime(const std::unique_ptr<AppData> &appData)
{
    auto currentFrame = static_cast<float>(glfwGetTime());
    appData->deltaTime = currentFrame - appData->lastFrame;
    appData->lastFrame = currentFrame;
}