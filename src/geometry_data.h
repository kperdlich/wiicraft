#pragma once

#include "vector3f.h"

namespace core {

struct Vertex3f
{
    float x, y, z;
};

struct Box
{
    math::Vector3f vertices[8];
};

}



