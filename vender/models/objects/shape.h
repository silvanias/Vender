#pragma once

class AbstractShape
{
public:
    virtual std::tuple<unsigned int, unsigned int> setupBuffers() = 0;
    virtual ~AbstractShape() = default;

protected:
    std::tuple<unsigned int, unsigned int> reserveVertexMemory(size_t BUFFER_SIZE)
    {
        unsigned int VBO;
        unsigned int VAO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, BUFFER_SIZE, nullptr, GL_STATIC_DRAW);
        return {VBO, VAO};
    }
};