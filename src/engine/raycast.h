#pragma once

#include <vector>
#include "aabb.h"
#include "vector3f.h"
#include "renderer.h"
#include "game/chunksection.h"

namespace core {

struct RayHitResult
{
    float Distance;
    core::AABB Entity; // Todo use real entity
    math::Vector3f HitLocation;
    math::Vector3f Normal;
};


bool Raycast(const std::map<std::pair<int32_t, int32_t>, std::shared_ptr<wiicraft::ChunkSection>> &entites, const math::Vector3f &origin, math::Vector3f direction, const float maxDistance, RayHitResult& hitResult);
bool Raycast(std::vector<AABB> &entites, const math::Vector3f &origin, math::Vector3f direction, const float maxDistance, RayHitResult& hitResult);
math::Vector3f GetAABBIntersectionPointSurfaceNormal(const math::Vector3f& intersectionPoint, const core::AABB& aabb);

}
