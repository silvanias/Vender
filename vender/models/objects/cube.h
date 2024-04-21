#pragma once

#include <array>
#include <tuple>

#include "shape.h"

struct CubeData
{
    static constexpr std::array<float, 108> vertPos = {
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

    static constexpr std::array<float, 108> vertNorm = {
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

    static constexpr std::array<float, 72> texCoords = {
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

class CubeDefault : public AbstractShape
{
public:
    CubeDefault()
    {
        setupBuffers();
    }

    void render() const override
    {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    ~CubeDefault() override
    {
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);
    };

protected:
    size_t getVertexDataSize() const override
    {
        return CubeData::vertPosSize;
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
        glBufferSubData(GL_ARRAY_BUFFER, 0, CubeData::vertPosSize, CubeData::vertPos.data());
    }

private:
    unsigned int VBO;
    unsigned int VAO;
};

class CubeNorm : public AbstractShape
{
public:
    CubeNorm()
    {
        setupBuffers();
    }

    void render() const override
    {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    ~CubeNorm() override
    {
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);
    };

protected:
    size_t getVertexDataSize() const override
    {
        return CubeData::vertPosSize + CubeData::vertNormSize + CubeData::texCoordSize;
    }

    void setupVAO(unsigned int _VAO) override
    {
        VAO = _VAO;
        glBindVertexArray(VAO);
        enableVertexAttribute(0, 3, 3 * sizeof(float), 0);
        enableVertexAttribute(1, 3, 3 * sizeof(float), CubeData::vertPosSize);
    }

    void setupVBO(unsigned int _VBO) override
    {
        VBO = _VBO;
        glBufferSubData(GL_ARRAY_BUFFER, 0, CubeData::vertPosSize, CubeData::vertPos.data());
        glBufferSubData(GL_ARRAY_BUFFER, CubeData::vertPosSize, CubeData::vertNormSize, CubeData::vertNorm.data());
    }

private:
    unsigned int VBO;
    unsigned int VAO;
};

class CubeTex : public AbstractShape
{
public:
    CubeTex()
    {
        setupBuffers();
    }

    void render() const override
    {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    ~CubeTex() override
    {
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);
    };

protected:
    size_t getVertexDataSize() const override
    {
        return CubeData::vertPosSize + CubeData::vertNormSize + CubeData::texCoordSize;
    }

    void setupVAO(unsigned int _VAO) override
    {
        VAO = _VAO;
        glBindVertexArray(VAO);
        enableVertexAttribute(0, 3, 3 * sizeof(float), 0);
        enableVertexAttribute(1, 3, 3 * sizeof(float), CubeData::vertPosSize);
        enableVertexAttribute(2, 2, 2 * sizeof(float), CubeData::vertPosSize + CubeData::vertNormSize);
    }

    void setupVBO(unsigned int _VBO) override
    {
        VBO = _VBO;
        glBufferSubData(GL_ARRAY_BUFFER, 0, CubeData::vertPosSize, CubeData::vertPos.data());
        glBufferSubData(GL_ARRAY_BUFFER, CubeData::vertPosSize, CubeData::vertNormSize, CubeData::vertNorm.data());
        glBufferSubData(GL_ARRAY_BUFFER, CubeData::vertPosSize + CubeData::vertNormSize, CubeData::texCoordSize, CubeData::texCoords.data());
    }

private:
    unsigned int VBO;
    unsigned int VAO;
};