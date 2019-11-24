#include <float.h>
#include "raycast.h"
#include <math.h>
#include <sstream>

bool core::Raycast(const std::vector<core::AABB*>& entites, const math::Vector3f& origin, math::Vector3f direction, const float maxDistance, RayHitResult& hitResult)
{    
    direction.Normalize();

    for (auto& entity : entites)
    {
        const float t1 = (entity->GetMin().X() - origin.X()) / direction.X();
        const float t2 = (entity->GetMax().X() - origin.X()) / direction.X();
        const float t3 = (entity->GetMin().Y() - origin.Y()) / direction.Y();
        const float t4 = (entity->GetMax().Y() - origin.Y()) / direction.Y();
        const float t5 = (entity->GetMin().Z() - origin.Z()) / direction.Z();
        const float t6 = (entity->GetMax().Z() - origin.Z()) / direction.Z();

        const float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
        const float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

        if (tmin > maxDistance)
        {
            return false;
        }

        if (tmax < 0)
        {
            continue; //return false;
        }

        if (tmin > tmax)
        {
            continue; //return false;
        }

        hitResult.Entity = entity;
        hitResult.Distance = tmin;
        return true;
    }

    return false;
}
