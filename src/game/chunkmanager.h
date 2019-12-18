#pragma once

#include <map>
#include <vector>
#include "aabb.h"
#include "BlockManager.h"
#include "eventmanager.h"
#include "chunksection.h"
#include "ChunkLoaderMultiplayer.h"
#include "serializationJob.h"
#include "raycast.h"
#include "renderer.h"

namespace wiicraft {

class ChunkManager
{
public:
    static constexpr int8_t CHUNK_CASH_X = 5;
    static constexpr int8_t CHUNK_CASH_Y = 5;

    ChunkManager();
    ~ChunkManager();
    ChunkManager(const ChunkManager&) = delete;
    ChunkManager& operator = (const ChunkManager&) = delete;
    ChunkManager(ChunkManager&&) = delete;
    ChunkManager& operator = (ChunkManager&&) = delete;

    void UpdateChunks(const math::Vector3f& currentPlayerPosition);
    void Render(renderer::Renderer& renderer);
    bool Raycast(const math::Vector3f &origin, math::Vector3f direction, const float maxDistance, core::RayHitResult &hitResult) const;
    std::shared_ptr<wiicraft::ChunkSection> GetChunk(const ChunkPosition &chunkPosition);
    std::vector<core::AABB> GetCollidableBlockAABBsAround(const math::Vector3f& position) const;
    std::vector<core::AABB> GetBlockAABBsAround(const math::Vector3f& position) const;
    std::vector<wiicraft::ChunkPosition> GenerateChunkMap(const math::Vector3f &worldPosition) const;
    inline uint32_t GetLoaderQueueCount();
    inline uint32_t GetSerializationQueueCount();

private:
    void OnSerializeChunk(core::IEventDataPtr eventData);
    void OnBlockChange(core::IEventDataPtr eventData);

private:
    wiicraft::BlockManager mBlockManager;
    std::map<ChunkPosition, std::shared_ptr<ChunkSection>> mChunksInLoadingProgress;
    std::map<ChunkPosition, std::shared_ptr<ChunkSection>> mChunkCache;
    wiicraft::ChunkLoaderMultiplayer mChunkLoaderJob;
    wiicraft::SerializationJob mChunkSerializationJob;
    wiicraft::ChunkPosition mPreviousUpdatedChunkPosition = {-1, -1};
};

inline uint32_t ChunkManager::GetLoaderQueueCount()
{
    return mChunkLoaderJob.GetQueueCount();
}

inline uint32_t ChunkManager::GetSerializationQueueCount()
{
    return mChunkSerializationJob.GetQueueCount();
}

}
