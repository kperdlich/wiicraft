#pragma once

#include <map>
#include <vector>
#include "aabb.h"
#include "BlockManager.h"
#include "chunksection.h"
#include "ChunkLoaderMultiplayer.h"
#include "eventmanager.h"
#include "raycast.h"
#include "renderer.h"
#include "serializationJob.h"

namespace wiicraft {

class Player;

class ChunkManager
{
public:
    static constexpr int8_t CHUNK_CACHE_X = 5;
    static constexpr int8_t CHUNK_CACHE_Y = 5;

    ChunkManager();
    ~ChunkManager();
    ChunkManager(const ChunkManager&) = delete;
    ChunkManager& operator = (const ChunkManager&) = delete;
    ChunkManager(ChunkManager&&) = delete;
    ChunkManager& operator = (ChunkManager&&) = delete;

    void UpdateChunksAround(const math::Vector3f &position);
    void Render(renderer::Renderer& renderer);
    bool Raycast(const math::Vector3f &origin, math::Vector3f direction, const float maxDistance, core::RayHitResult &hitResult) const;
    std::shared_ptr<wiicraft::ChunkSection> GetChunk(const ChunkPosition &chunkPosition);
    std::vector<core::AABB> GetCollidableBlockAABBsAround(const math::Vector3f& position) const;
    std::vector<wiicraft::ChunkPosition> GenerateChunkMap(const math::Vector3f &worldPosition) const;
    inline uint32_t GetLoaderQueueCount();
    inline uint32_t GetSerializationQueueCount();
    inline wiicraft::BlockManager& GetBlockManager();

private:
    void OnSerializeChunk(core::IEventDataPtr eventData);
    void OnBlockChange(core::IEventDataPtr eventData);

private:
    wiicraft::BlockManager mBlockManager;
    std::map<ChunkPosition, std::shared_ptr<ChunkSection>> mChunksInLoadingProgress;
    std::map<ChunkPosition, std::shared_ptr<ChunkSection>> mChunkCache;
    wiicraft::ChunkLoaderMultiplayer mChunkLoaderJob;
    wiicraft::SerializationJob mChunkSerializationJob;
    math::Vector3f mPreviousPlayerPos;
    bool mInitialMapLoaded;
};

inline uint32_t ChunkManager::GetLoaderQueueCount()
{
    return mChunkLoaderJob.GetQueueCount();
}

inline uint32_t ChunkManager::GetSerializationQueueCount()
{
    return mChunkSerializationJob.GetQueueCount();
}
inline BlockManager& ChunkManager::GetBlockManager()
{
    return mBlockManager;
}

}
