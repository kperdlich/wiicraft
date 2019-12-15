#include <float.h>
#include "raycast.h"
#include <cmath>
#include <sstream>
#include <algorithm>

bool core::Raycast(std::vector<core::AABB>& entites, const math::Vector3f& origin, math::Vector3f direction, const float maxDistance, RayHitResult& hitResult)
{    
    direction.Normalize();

    std::sort(entites.begin(), entites.end(), [&origin](const core::AABB& a, const core::AABB& b)
    {
        const float aDistance  = (a.GetCenter() - origin).Length();
        const float bDistance  = (b.GetCenter() - origin).Length();
        return aDistance < bDistance;
    });

    for (auto& entity : entites)
    {
        const float t1 = (entity.GetMin().X() - origin.X()) / direction.X();
        const float t2 = (entity.GetMax().X() - origin.X()) / direction.X();
        const float t3 = (entity.GetMin().Y() - origin.Y()) / direction.Y();
        const float t4 = (entity.GetMax().Y() - origin.Y()) / direction.Y();
        const float t5 = (entity.GetMin().Z() - origin.Z()) / direction.Z();
        const float t6 = (entity.GetMax().Z() - origin.Z()) / direction.Z();

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

bool core::Raycast(const std::map<std::pair<int32_t, int32_t>, std::shared_ptr<wiicraft::ChunkSection> > &chunkMap, const math::Vector3f &origin,
                   math::Vector3f direction, const float maxDistance, core::RayHitResult &hitResult)
{
    direction.Normalize();

    for (float i = 0; i < maxDistance; i += 0.1f)
    {
        const math::Vector3f& currentStepPos = origin + (direction * i);
        const wiicraft::ChunkPosition& currentStepChunkPos = wiicraft::ChunkSection::WorldPositionToChunkPosition(currentStepPos);
        const math::Vector3f& currentBlockPos = wiicraft::ChunkSection::WorldPositionToBlockPosition(currentStepPos, currentStepChunkPos);
        const wiicraft::ChunkPosition& currentBlockChunkPos = wiicraft::ChunkSection::WorldPositionToChunkPosition(currentBlockPos);
        const auto& chunkIt = chunkMap.find(currentBlockChunkPos);
        if (chunkIt != chunkMap.end())        {

            const std::pair<math::Vector3f, wiicraft::BlockType> block = chunkIt->second->GetBlockTypeByWorldPosition(currentBlockPos);
            if (block.second == wiicraft::BlockType::AIR)
                continue;

            const core::AABB entity(block.first, {0.5f, 0.5f, 0.5f});
            const float t1 = (entity.GetMin().X() - origin.X()) / direction.X();
            const float t2 = (entity.GetMax().X() - origin.X()) / direction.X();
            const float t3 = (entity.GetMin().Y() - origin.Y()) / direction.Y();
            const float t4 = (entity.GetMax().Y() - origin.Y()) / direction.Y();
            const float t5 = (entity.GetMin().Z() - origin.Z()) / direction.Z();
            const float t6 = (entity.GetMax().Z() - origin.Z()) / direction.Z();

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
    }

    return false;
}
