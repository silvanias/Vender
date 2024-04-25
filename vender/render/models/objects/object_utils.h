#pragma once

#include <array>
#include <memory>

#include "shape.h"

enum struct ObjectIdx
{
    cubeNorm = 0,
    cubeTex = 1,
    pyramidNorm = 2,
    pyramidTex = 3,
    lightCube = 4,
};

std::array<std::unique_ptr<AbstractShape>, 5> createObjects();