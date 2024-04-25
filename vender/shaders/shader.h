#pragma once

#include <string>
#include <array>
#include <memory>

#include <glad/glad.h>
#include "models/lighting/light.h"

enum struct ShaderIdx
{
    generic = 0,
    tex = 1,
    light = 2,
};

class Shader
{
public:
    // The program ID
    unsigned int ID;

    // constructor reads and builds the shader
    Shader(const char *vertexPath, const char *fragmentPath);

    // Use/activate the shader
    void use() const;
    void del() const;

    // Utility uniform functions
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec2(const std::string &name, const glm::vec2 &value) const;
    void setVec2(const std::string &name, float x, float y) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setVec3(const std::string &name, float x, float y, float z) const;
    void setVec4(const std::string &name, const glm::vec4 &value) const;
    void setVec4(const std::string &name, float x, float y, float z, float w) const;
    void setMat2(const std::string &name, const glm::mat2 &mat) const;
    void setMat3(const std::string &name, const glm::mat3 &mat) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;

private:
    void checkCompileErrors(unsigned int shader, const std::string &type) const;
};

std::array<std::unique_ptr<Shader>, 3> loadShaders();
void configureShaders(std::array<std::unique_ptr<Shader>, 3> &shaders);
void setShaderLighting(const Shader &shader, const Light &light);
void setShaderMVP(const Shader &shader, const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection);
void bindTextures(unsigned int diffuseMap, unsigned int specularMap);