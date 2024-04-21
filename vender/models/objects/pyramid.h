#pragma once

#include <array>
#include <tuple>

#include "shape.h"

class AbstractPyramid : public AbstractShape
{
protected:
    void setupBuffers() override
    {
        const auto BUFFER_SIZE = getVertexDataSize();
        auto [VBO, VAO] = reserveVertexMemory(BUFFER_SIZE);
        setupVAO(VAO);
        setupVBO(VBO, BUFFER_SIZE);
    };
    virtual size_t getVertexDataSize() const = 0;
    virtual void setupVAO(unsigned int VAO) = 0;
    virtual void setupVBO(unsigned int VBO, size_t BUFFER_SIZE) = 0;

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

protected:
    size_t getVertexDataSize() const override
    {
        return vertPosSize;
    }

    void setupVAO(unsigned int _VAO) override
    {
        VAO = _VAO;
        glBindVertexArray(VAO);
        enableVertexAttribute(0, 3, 3 * sizeof(float), 0);
    }

    void setupVBO(unsigned int _VBO, size_t BUFFER_SIZE) override
    {
        VBO = _VBO;
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertPosSize, vertPos.data());
    }

private:
    unsigned int VBO;
    unsigned int VAO;
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

    ~PyramidNorm() override
    {
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);
    }

protected:
    size_t getVertexDataSize() const override
    {
        return vertPosSize + vertNormSize + texCoordSize;
    }

    void setupVAO(unsigned int _VAO) override
    {
        VAO = _VAO;
        glBindVertexArray(VAO);
        enableVertexAttribute(0, 3, 3 * sizeof(float), 0);
        enableVertexAttribute(1, 3, 3 * sizeof(float), vertPosSize);
    }

    void setupVBO(unsigned int _VBO, size_t BUFFER_SIZE) override
    {
        VBO = _VBO;
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertPosSize, vertPos.data());
        glBufferSubData(GL_ARRAY_BUFFER, vertPosSize, vertNormSize, vertNorm.data());
    }

private:
    unsigned int VBO;
    unsigned int VAO;
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

protected:
    size_t getVertexDataSize() const override
    {
        return vertPosSize + vertNormSize + texCoordSize;
    }

    void setupVAO(unsigned int _VAO) override
    {
        VAO = _VAO;
        glBindVertexArray(VAO);
        enableVertexAttribute(0, 3, 3 * sizeof(float), 0);
        enableVertexAttribute(1, 3, 3 * sizeof(float), vertPosSize);
        enableVertexAttribute(2, 2, 2 * sizeof(float), vertPosSize + vertNormSize);
    }

    void setupVBO(unsigned int _VBO, size_t BUFFER_SIZE) override
    {
        VBO = _VBO;
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertPosSize, vertPos.data());
        glBufferSubData(GL_ARRAY_BUFFER, vertPosSize, vertNormSize, vertNorm.data());
        glBufferSubData(GL_ARRAY_BUFFER, vertPosSize + vertNormSize, texCoordSize, texCoords.data());
    }

private:
    unsigned int VBO;
    unsigned int VAO;
};