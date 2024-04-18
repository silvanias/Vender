#pragma once

class AbstractShape
{
public:
    virtual std::tuple<unsigned int, unsigned int> setupBuffers() = 0;
    virtual ~AbstractShape() = default;
};
