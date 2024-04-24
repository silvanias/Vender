#pragma once

#include <array>
#include <memory>

#include "shape.h"
#include "cube/cube.h"
#include "pyramid/pyramid.h"

enum ObjectIdx
{
    cubeNorm,
    cubeTex,
    pyramidNorm,
    pyramidTex,
    lightCube,
};

std::array<std::unique_ptr<AbstractShape>, 5> createObjects()
{
    std::array<std::unique_ptr<AbstractShape>, 5> objects = {
        std::make_unique<CubeNorm>(),
        std::make_unique<CubeTex>(),
        std::make_unique<PyramidNorm>(),
        std::make_unique<PyramidTex>(),
        std::make_unique<CubeDefault>()};
    return objects;
};
