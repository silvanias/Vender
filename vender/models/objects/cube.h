#pragma once

#include <array>
#include <tuple>

#include "shape.h"

class AbstractCube : public AbstractShape
{
public:
    ~AbstractCube() override = default;

protected:
    const std::array<float, 108> vertPos = {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,

        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,

        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,

        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f};

    const std::array<float, 108> vertNorm = {
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,

        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,

        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,

        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,

        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f};

    const std::array<float, 72> texCoords = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,

        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,

        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f};

    static constexpr size_t vertPosSize = sizeof(vertPos);
    static constexpr size_t vertNormSize = sizeof(vertNorm);
    static constexpr size_t texCoordSize = sizeof(texCoords);
};

class CubeDefault : public AbstractCube
{
public:
    std::tuple<unsigned int, unsigned int> setupBuffers() override
    {
        const auto BUFFER_SIZE = vertPosSize;

        auto [VBO, VAO] = reserveVertexMemory(BUFFER_SIZE);

        glBufferSubData(GL_ARRAY_BUFFER, 0, vertPosSize, vertPos.data());

        glBindVertexArray(VAO);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid *)nullptr);
        glEnableVertexAttribArray(0);

        return {VBO, VAO};
    };
};

class CubeNorm : public AbstractCube
{
public:
    std::tuple<unsigned int, unsigned int> setupBuffers() override
    {
        const auto BUFFER_SIZE = vertPosSize + vertNormSize;
        const auto normOffset = vertPosSize;

        auto [VBO, VAO] = reserveVertexMemory(BUFFER_SIZE);

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

class CubeTex : public AbstractCube
{
public:
    std::tuple<unsigned int, unsigned int> setupBuffers() override
    {
        auto BUFFER_SIZE = vertPosSize + vertNormSize + texCoordSize;
        auto normOffset = vertPosSize;
        auto texCoordsOffset = normOffset + vertNormSize;

        auto [VBO, VAO] = reserveVertexMemory(BUFFER_SIZE);

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