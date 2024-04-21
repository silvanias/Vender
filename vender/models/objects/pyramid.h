#pragma once

#include <array>
#include <tuple>

#include "shape.h"

struct PyramidData
{
    static constexpr std::array<float, 54> vertPos = {
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

    static constexpr std::array<float, 54> vertNorm = {
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

    static constexpr std::array<float, 36> texCoords = {
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

class PyramidDefault : public AbstractShape
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

protected:
    size_t getVertexDataSize() const override
    {
        return PyramidData::vertPosSize;
    }

    void setupVAO(unsigned int _VAO) override
    {
        VAO = _VAO;
        glBindVertexArray(VAO);
        enableVertexAttribute(0, 3, 3 * sizeof(float), 0);
    }

    void setupVBO(unsigned int _VBO) override
    {
        VBO = _VBO;
        glBufferSubData(GL_ARRAY_BUFFER, 0, PyramidData::vertPosSize, PyramidData::vertPos.data());
    }

private:
    unsigned int VBO;
    unsigned int VAO;
};

class PyramidNorm : public AbstractShape
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

    ~PyramidNorm() override
    {
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);
    }

protected:
    size_t getVertexDataSize() const override
    {
        return PyramidData::vertPosSize + PyramidData::vertNormSize + PyramidData::texCoordSize;
    }

    void setupVAO(unsigned int _VAO) override
    {
        VAO = _VAO;
        glBindVertexArray(VAO);
        enableVertexAttribute(0, 3, 3 * sizeof(float), 0);
        enableVertexAttribute(1, 3, 3 * sizeof(float), PyramidData::vertPosSize);
    }

    void setupVBO(unsigned int _VBO) override
    {
        VBO = _VBO;
        glBufferSubData(GL_ARRAY_BUFFER, 0, PyramidData::vertPosSize, PyramidData::vertPos.data());
        glBufferSubData(GL_ARRAY_BUFFER, PyramidData::vertPosSize, PyramidData::vertNormSize, PyramidData::vertNorm.data());
    }

private:
    unsigned int VBO;
    unsigned int VAO;
};

class PyramidTex : public AbstractShape
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

protected:
    size_t getVertexDataSize() const override
    {
        return PyramidData::vertPosSize + PyramidData::vertNormSize + PyramidData::texCoordSize;
    }

    void setupVAO(unsigned int _VAO) override
    {
        VAO = _VAO;
        glBindVertexArray(VAO);
        enableVertexAttribute(0, 3, 3 * sizeof(float), 0);
        enableVertexAttribute(1, 3, 3 * sizeof(float), PyramidData::vertPosSize);
        enableVertexAttribute(2, 2, 2 * sizeof(float), PyramidData::vertPosSize + PyramidData::vertNormSize);
    }

    void setupVBO(unsigned int _VBO) override
    {
        VBO = _VBO;
        glBufferSubData(GL_ARRAY_BUFFER, 0, PyramidData::vertPosSize, PyramidData::vertPos.data());
        glBufferSubData(GL_ARRAY_BUFFER, PyramidData::vertPosSize, PyramidData::vertNormSize, PyramidData::vertNorm.data());
        glBufferSubData(GL_ARRAY_BUFFER, PyramidData::vertPosSize + PyramidData::vertNormSize, PyramidData::texCoordSize, PyramidData::texCoords.data());
    }

private:
    unsigned int VBO;
    unsigned int VAO;
};