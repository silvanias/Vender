#pragma once

class AbstractShape
{
public:
    AbstractShape() = default;
    virtual void render() const = 0;
    virtual ~AbstractShape() = default;

protected:
    virtual void setupBuffers() = 0;

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

    void enableVertexAttribute(unsigned int index, unsigned int numComponents, size_t stride, size_t offset) const
    {
        glVertexAttribPointer(index, numComponents, GL_FLOAT, GL_FALSE, stride, (GLvoid *)offset);
        glEnableVertexAttribArray(index);
    }
};
