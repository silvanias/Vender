#pragma once

#include <glad/glad.h>
class AbstractShape
{
public:
    virtual void render() const = 0;
    virtual ~AbstractShape() = default;

protected:
    void setupBuffers()
    {
        const auto BUFFER_SIZE = getVertexDataSize();
        auto [VAO, VBO] = reserveVertexMemory(BUFFER_SIZE);
        setupVAO(VAO);
        setupVBO(VBO);
    };

    virtual size_t getVertexDataSize() const = 0;

    std::tuple<unsigned int, unsigned int> reserveVertexMemory(size_t BUFFER_SIZE)
    {
        unsigned int VAO;
        unsigned int VBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, BUFFER_SIZE, nullptr, GL_STATIC_DRAW);
        return {VAO, VBO};
    }

    virtual void setupVAO(unsigned int VAO) = 0;
    virtual void setupVBO(unsigned int VBO) = 0;

    void enableVertexAttribute(unsigned int index, unsigned int numComponents, size_t stride, size_t offset) const
    {
        glVertexAttribPointer(index, numComponents, GL_FLOAT, GL_FALSE, stride, (GLvoid *)offset);
        glEnableVertexAttribArray(index);
    }
};
