#pragma once

#include <array>
#include <tuple>

#include "shape.h"

class AbstractPyramid : public AbstractShape
{
public:
    ~AbstractPyramid() override = default;

protected:
    const std::array<float, 54> vertPos = {
        0.0f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,

        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, -0.5f,

        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        0.0f, 0.5f, 0.0f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,

        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,

        0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f};

    const std::array<float, 54> vertNorm = {
        0.0f, 0.71f, 0.71f,
        0.0f, 0.71f, 0.71f,
        0.0f, 0.71f, 0.71f,

        0.71f, 0.71f, 0.0f,
        0.71f, 0.71f, 0.0f,
        0.71f, 0.71f, 0.0f,

        0.0f, 0.71f, -0.71f,
        0.0f, 0.71f, -0.71f,
        0.0f, 0.71f, -0.71f,

        -0.71f, 0.71f, 0.0f,
        -0.71f, 0.71f, 0.0f,
        -0.71f, 0.71f, 0.0f,

        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,

        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f};

    const std::array<float, 36> texCoords = {
        0.5f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        0.5f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        0.5f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        0.5f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f};
};

class PyramidDefault : public AbstractPyramid
{
public:
    std::tuple<unsigned int, unsigned int> setupBuffers() override
    {
        unsigned int VBO;
        unsigned int VAO;
        glGenVertexArrays(1, &VAO);

        const auto vertPosSize = sizeof(vertPos);

        const auto BUFFER_SIZE = vertPosSize;

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, BUFFER_SIZE, nullptr, GL_STATIC_DRAW);

        glBufferSubData(GL_ARRAY_BUFFER, 0, vertPosSize, vertPos.data());

        glBindVertexArray(VAO);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid *)nullptr);
        glEnableVertexAttribArray(0);

        return {VBO, VAO};
    };
};

class PyramidNorm : public AbstractPyramid
{
public:
    std::tuple<unsigned int, unsigned int> setupBuffers() override
    {
        unsigned int VBO;
        unsigned int VAO;
        glGenVertexArrays(1, &VAO);

        const auto vertPosSize = sizeof(vertPos);
        const auto vertNormSize = sizeof(vertNorm);

        const auto BUFFER_SIZE = vertPosSize + vertNormSize;
        const auto normOffset = vertPosSize;

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, BUFFER_SIZE, nullptr, GL_STATIC_DRAW);

        glBufferSubData(GL_ARRAY_BUFFER, 0, vertPosSize, vertPos.data());
        glBufferSubData(GL_ARRAY_BUFFER, normOffset, vertNormSize, vertNorm.data());

        glBindVertexArray(VAO);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid *)nullptr);
        glEnableVertexAttribArray(0);
        // Normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid *)normOffset);
        glEnableVertexAttribArray(1);

        return {VBO, VAO};
    };
};

class PyramidTex : public AbstractPyramid
{
public:
    std::tuple<unsigned int, unsigned int> setupBuffers() override
    {
        unsigned int VBO;
        unsigned int VAO;
        glGenVertexArrays(1, &VAO);

        auto vertPosSize = sizeof(vertPos);
        auto vertNormSize = sizeof(vertNorm);
        auto texCoordSize = sizeof(texCoords);

        auto BUFFER_SIZE = vertPosSize + vertNormSize + texCoordSize;
        auto normOffset = vertPosSize;
        auto texCoordsOffset = normOffset + vertNormSize;

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, BUFFER_SIZE, nullptr, GL_STATIC_DRAW);

        glBufferSubData(GL_ARRAY_BUFFER, 0, vertPosSize, vertPos.data());
        glBufferSubData(GL_ARRAY_BUFFER, normOffset, vertNormSize, vertNorm.data());
        glBufferSubData(GL_ARRAY_BUFFER, texCoordsOffset, texCoordSize, texCoords.data());

        glBindVertexArray(VAO);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid *)nullptr);
        glEnableVertexAttribArray(0);
        // Normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid *)normOffset);
        glEnableVertexAttribArray(1);
        // Texture attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (GLvoid *)texCoordsOffset);
        glEnableVertexAttribArray(2);

        return {VBO, VAO};
    }
};