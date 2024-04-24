#pragma once

#include <array>
#include <memory>

#include "shape.h"

enum ObjectIdx
{
    cubeNorm,
    cubeTex,
    pyramidNorm,
    pyramidTex,
    lightCube,
};

std::array<std::unique_ptr<AbstractShape>, 5> createObjects();