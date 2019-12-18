#include "chunkmanager.h"
#include "camera.h"
#include "core.h"
#include "EventDataSerializeChunk.h"
#include "EventDataChangeBlock.h"

wiicraft::ChunkManager::ChunkManager()
{
    for (int8_t x = 0; x < CHUNK_CACHE_X; ++x)
    {
        for (int8_t y = 0; y < CHUNK_CACHE_Y; ++y)
        {
            std::shared_ptr<wiicraft::ChunkSection> chunkSection = std::make_shared<wiicraft::ChunkSection>();
            chunkSection->SetPosition({x, y});
            chunkSection->SetTo(wiicraft::BlockType::AIR);
            chunkSection->SetLoaded(false);
            mChunkCache[chunkSection->GetPosition()] = chunkSection;
        }
    }

    ASSERT(mChunkCache.size() == CHUNK_CACHE_X * CHUNK_CACHE_Y);
    ASSERT(mChunksInLoadingProgress.size() == 0);

    mChunkLoaderJob.Start();
    mChunkSerializationJob.Start();

   core::IEventManager::Get()->AddListener(fastdelegate::MakeDelegate(this, &ChunkManager::OnSerializeChunk), EventDataSerializeChunk::EventType);
   core::IEventManager::Get()->AddListener(fastdelegate::MakeDelegate(this, &ChunkManager::OnBlockChange), EventDataChangeBlock::EventType);
}

wiicraft::ChunkManager::~ChunkManager()
{
    core::IEventManager::Get()->RemoveListener(fastdelegate::MakeDelegate(this, &ChunkManager::OnSerializeChunk), EventDataSerializeChunk::EventType);
    core::IEventManager::Get()->RemoveListener(fastdelegate::MakeDelegate(this, &ChunkManager::OnBlockChange), EventDataChangeBlock::EventType);
    mChunkLoaderJob.Stop();
    mChunkSerializationJob.Stop();
}

void wiicraft::ChunkManager::UpdateChunks(const math::Vector3f &currentPlayerPosition)
{
    ASSERT(mChunkCache.size() + mChunksInLoadingProgress.size() == CHUNK_CACHE_X * CHUNK_CACHE_Y);

    for (auto chunkIt = mChunksInLoadingProgress.begin(); chunkIt != mChunksInLoadingProgress.end();)
    {
        if (chunkIt->second->IsLoaded())
        {
            mChunkCache[chunkIt->second->GetPosition()] = chunkIt->second;
            chunkIt = mChunksInLoadingProgress.erase(chunkIt);
        }
        else
        {
            ++chunkIt;
        }
    }

    ASSERT(mChunkCache.size() + mChunksInLoadingProgress.size() == CHUNK_CACHE_X * CHUNK_CACHE_Y);
    std::vector<std::shared_ptr<ChunkSection>> newChunkCache;
    std::vector<wiicraft::ChunkPosition> chunkmap = GenerateChunkMap(currentPlayerPosition);
    ASSERT(chunkmap.size() == CHUNK_CACHE_X * CHUNK_CACHE_Y);
    for (auto chunkMapIt = chunkmap.begin(); chunkMapIt != chunkmap.end();)
    {
        auto chunkIt = mChunkCache.find(*chunkMapIt);
        if (chunkIt != mChunkCache.end() && chunkIt->second->IsLoaded())
        {
            newChunkCache.push_back(chunkIt->second);
            chunkMapIt = chunkmap.erase(chunkMapIt);
            mChunkCache.erase(chunkIt);
        }
        else
        {
            auto findIt = mChunksInLoadingProgress.find(*chunkMapIt);
            if (findIt != mChunksInLoadingProgress.end())
            {
                chunkMapIt = chunkmap.erase(chunkMapIt);
            }
            else
            {
                ++chunkMapIt;
            }
        }
    }
    std::vector<std::shared_ptr<wiicraft::ChunkSection>> loadingList;
    for (auto chunkIt = mChunkCache.begin(); chunkIt != mChunkCache.end();)
    {
        const wiicraft::ChunkPosition& newChunkPos = chunkmap.back();
        chunkIt->second->SetTo(wiicraft::BlockType::AIR);
        chunkIt->second->SetPosition(newChunkPos);
        chunkIt->second->SetLoaded(false);
        chunkmap.pop_back();
        loadingList.push_back(chunkIt->second);
        mChunkLoaderJob.Add(chunkIt->second);
        chunkIt = mChunkCache.erase(chunkIt);
    }

    for(auto& cs : loadingList)
    {
        mChunksInLoadingProgress[cs->GetPosition()] = cs;
    }
    for(auto& cs : newChunkCache)
    {
        mChunkCache[cs->GetPosition()] = cs;
    }

    ASSERT(mChunkCache.size() + mChunksInLoadingProgress.size() == CHUNK_CACHE_X * CHUNK_CACHE_Y);

}

void wiicraft::ChunkManager::Render(renderer::Renderer &renderer)
{
    for (auto& c : mChunkCache)
    {
        auto cs = c.second;
        renderer.LoadModelViewMatrix(renderer.GetCamera()->GetViewMatrix3x4() * math::Matrix3x4::Identity());
        renderer.DrawRay(cs->GetWorldPosition() + math::Vector3f{wiicraft::ChunkSection::CHUNK_SIZE * 0.5f, 256.0f -8.0f,wiicraft::ChunkSection::CHUNK_SIZE * 0.5f}, math::Vector3f::Left * 10.0f, renderer::ColorRGBA::RED);
        renderer.DrawRay(cs->GetWorldPosition() + math::Vector3f{wiicraft::ChunkSection::CHUNK_SIZE * 0.5f, 256.0f-8.0f, wiicraft::ChunkSection::CHUNK_SIZE * 0.5f}, math::Vector3f::Up * 10.0f, renderer::ColorRGBA::GREEN);
        renderer.DrawRay(cs->GetWorldPosition() + math::Vector3f{wiicraft::ChunkSection::CHUNK_SIZE * 0.5f, 256.0f-8.0f, wiicraft::ChunkSection::CHUNK_SIZE * 0.5f}, math::Vector3f::Forward * 10.0f, renderer::ColorRGBA::BLUE);
        cs->Render(renderer, mBlockManager);
    }
}

bool wiicraft::ChunkManager::Raycast(const math::Vector3f &origin, math::Vector3f direction, const float maxDistance, core::RayHitResult &hitResult) const
{
    return core::Raycast(mChunkCache, origin, direction, maxDistance, hitResult);
}

std::shared_ptr<wiicraft::ChunkSection> wiicraft::ChunkManager::GetChunk(const ChunkPosition& chunkPosition)
{
    const auto& chunkSectionIt = mChunkCache.find(chunkPosition);
    if (chunkSectionIt != mChunkCache.end())
    {
        return chunkSectionIt->second;
    }
    return nullptr;
}

std::vector<core::AABB> wiicraft::ChunkManager::GetCollidableBlockAABBsAround(const math::Vector3f &position) const
{
    std::vector<core::AABB> aabbs;
    math::Vector3f blockPosition = wiicraft::ChunkSection::WorldPositionToBlockPosition(position, wiicraft::ChunkSection::WorldPositionToChunkPosition(position));
    math::Vector3f start = {blockPosition.X() - 1.0f, blockPosition.Y() - 1.0f, blockPosition.Z() - 1.0f};
    for (uint8_t x = 0; x < 3; ++x)
    {
        for (uint8_t y = 0; y < 3; ++y)
        {
            for (uint8_t z = 0; z < 3; ++z)
            {
                const math::Vector3f& pos = {start.X() +(x*ChunkSection::BLOCK_HALF_SIZE*2.0f), start.Y() +(y*ChunkSection::BLOCK_HALF_SIZE*2.0f), start.Z() +(z*ChunkSection::BLOCK_HALF_SIZE*2.0f)};
                const wiicraft::ChunkPosition& chunkPos = wiicraft::ChunkSection::WorldPositionToChunkPosition(pos);
                auto csIt = mChunkCache.find(chunkPos);
                if (csIt != mChunkCache.end())
                {
                    const auto& blockPositionType = csIt->second->GetBlockTypeByWorldPosition(pos);
                    if (blockPositionType.second != wiicraft::BlockType::AIR)
                    {
                        aabbs.push_back({blockPositionType.first, {ChunkSection::BLOCK_HALF_SIZE, ChunkSection::BLOCK_HALF_SIZE, ChunkSection::BLOCK_HALF_SIZE}});
                    }
                }
            }
        }
    }
    return aabbs;
}

std::vector<core::AABB> wiicraft::ChunkManager::GetBlockAABBsAround(const math::Vector3f &position) const
{
    std::vector<core::AABB> aabbs;
    math::Vector3f blockPosition = wiicraft::ChunkSection::WorldPositionToBlockPosition(position, wiicraft::ChunkSection::WorldPositionToChunkPosition(position));
    math::Vector3f start = {blockPosition.X() - 1.0f, blockPosition.Y() - 1.0f, blockPosition.Z() - 1.0f};
    for (uint8_t x = 0; x < 3; ++x)
    {
        for (uint8_t y = 0; y < 3; ++y)
        {
            for (uint8_t z = 0; z < 3; ++z)
            {
                const math::Vector3f& pos = {start.X() +(x*ChunkSection::BLOCK_HALF_SIZE*2.0f), start.Y() +(y*ChunkSection::BLOCK_HALF_SIZE*2.0f), start.Z() +(z*ChunkSection::BLOCK_HALF_SIZE*2.0f)};
                const wiicraft::ChunkPosition& chunkPos = wiicraft::ChunkSection::WorldPositionToChunkPosition(pos);
                auto csIt = mChunkCache.find(chunkPos);
                if (csIt != mChunkCache.end())
                {
                    const auto& blockPositionType = csIt->second->GetBlockTypeByWorldPosition(pos);
                    aabbs.push_back({blockPositionType.first, {ChunkSection::BLOCK_HALF_SIZE, ChunkSection::BLOCK_HALF_SIZE, ChunkSection::BLOCK_HALF_SIZE}});
                }
            }
        }
    }
    return aabbs;
}

std::vector<wiicraft::ChunkPosition> wiicraft::ChunkManager::GenerateChunkMap(const math::Vector3f &worldPosition) const
{
    const ChunkPosition& chunkPosition = wiicraft::ChunkSection::WorldPositionToChunkPosition(worldPosition);
    std::vector<ChunkPosition> chunkList;

    int32_t x = chunkPosition.first - (CHUNK_CACHE_X/ 2);
    int32_t z = chunkPosition.second + (CHUNK_CACHE_Y / 2);

    for (int32_t i = 0; i < CHUNK_CACHE_X; i++)
    {
        for (int32_t j = 0; j < CHUNK_CACHE_Y; j++)
        {
            chunkList.push_back({x + i, z - j});
        }
    }

    //ASSERT(chunkList.size() == 100);
    return chunkList;
}

void wiicraft::ChunkManager::OnSerializeChunk(core::IEventDataPtr eventData)
{
    std::shared_ptr<EventDataSerializeChunk> chunkData = std::static_pointer_cast<EventDataSerializeChunk>(eventData);
    mChunkSerializationJob.Add(chunkData->GetCompressedChunk());
}

void wiicraft::ChunkManager::OnBlockChange(core::IEventDataPtr eventData)
{
    std::shared_ptr<EventDataChangeBlock> blockData = std::static_pointer_cast<EventDataChangeBlock>(eventData);
    math::Vector3f worldPos = math::Vector3f(blockData->GetX(), blockData->GetY(), blockData->GetZ());
    wiicraft::ChunkPosition cp = wiicraft::ChunkSection::WorldPositionToChunkPosition(worldPos);
    auto chunkSection = GetChunk(cp);
    if (chunkSection)
    {
        chunkSection->SetBlock(wiicraft::ChunkSection::BlockWorldPositionToLocalChunkPosition(
                               chunkSection->WorldPositionToBlockPosition(worldPos)),
                               static_cast<BlockType>(blockData->GetBlockType()));
    }

}
