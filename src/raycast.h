#pragma once

#include <vector>
#include "aabb.h"
#include "vector3f.h"
#include "renderer.h"

namespace core {

struct RayHitResult
{
    float Distance;
    core::AABB* Entity; // Todo use real entity
};

bool Raycast(const std::vector<core::AABB*>& entites, const math::Vector3f &origin, math::Vector3f direction, const float maxDistance, RayHitResult& hitResult);

}
