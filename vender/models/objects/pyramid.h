#pragma once

#include <array>
#include <tuple>

#include "shape.h"

class AbstractPyramid : public AbstractShape
{
public:
    void render() const override = 0;
    ~AbstractPyramid() override = default;

protected:
    void setupBuffers() override = 0;
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

    static constexpr size_t vertPosSize = sizeof(vertPos);
    static constexpr size_t vertNormSize = sizeof(vertNorm);
    static constexpr size_t texCoordSize = sizeof(texCoords);
};

class PyramidDefault : public AbstractPyramid
{
public:
    PyramidDefault()
    {
        setupBuffers();
    }

    void render() const override
    {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 18);
    }

    ~PyramidDefault()
    {
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);
    }

private:
    unsigned int VBO;
    unsigned int VAO;

    void setupBuffers() override
    {
        const auto BUFFER_SIZE = vertPosSize;
        auto [_VBO, _VAO] = reserveVertexMemory(BUFFER_SIZE);
        VBO = _VBO;
        VAO = _VAO;

        glBufferSubData(GL_ARRAY_BUFFER, 0, vertPosSize, vertPos.data());
        glBindVertexArray(VAO);
        enableVertexAttribute(0, 3, 3 * sizeof(float), 0);
    };
};

class PyramidNorm : public AbstractPyramid
{
public:
    PyramidNorm()
    {
        setupBuffers();
    }

    void render() const override
    {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 18);
    }

    ~PyramidNorm()
    {
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);
    }

private:
    unsigned int VBO;
    unsigned int VAO;

    void setupBuffers() override
    {
        const auto BUFFER_SIZE = vertPosSize + vertNormSize;
        const auto normOffset = vertPosSize;
        auto [_VBO, _VAO] = reserveVertexMemory(BUFFER_SIZE);

        VBO = _VBO;
        VAO = _VAO;
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertPosSize, vertPos.data());
        glBufferSubData(GL_ARRAY_BUFFER, normOffset, vertNormSize, vertNorm.data());

        glBindVertexArray(VAO);

        enableVertexAttribute(0, 3, 3 * sizeof(float), 0);
        enableVertexAttribute(1, 3, 3 * sizeof(float), normOffset);
    };
};

class PyramidTex : public AbstractPyramid
{
public:
    PyramidTex()
    {
        setupBuffers();
    }

    void render() const override
    {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 18);
    }

    ~PyramidTex()
    {
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);
    }

private:
    unsigned int VBO;
    unsigned int VAO;

    void setupBuffers() override
    {
        auto BUFFER_SIZE = vertPosSize + vertNormSize + texCoordSize;
        auto normOffset = vertPosSize;
        auto texCoordsOffset = normOffset + vertNormSize;

        auto [_VBO, _VAO] = reserveVertexMemory(BUFFER_SIZE);
        VBO = _VBO;
        VAO = _VAO;

        glBufferSubData(GL_ARRAY_BUFFER, 0, vertPosSize, vertPos.data());
        glBufferSubData(GL_ARRAY_BUFFER, normOffset, vertNormSize, vertNorm.data());
        glBufferSubData(GL_ARRAY_BUFFER, texCoordsOffset, texCoordSize, texCoords.data());

        glBindVertexArray(VAO);

        enableVertexAttribute(0, 3, 3 * sizeof(float), 0);
        enableVertexAttribute(1, 3, 3 * sizeof(float), normOffset);
        enableVertexAttribute(2, 2, 2 * sizeof(float), texCoordsOffset);
    }
};