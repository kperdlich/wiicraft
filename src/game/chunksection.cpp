#include <math.h>
#include "chunksection.h"
#include "mathhelper.h"
#include "renderer.h"
#include "camera.h"
#include "vertexformat.h"
#include "BlockManager.h"

wiicraft::ChunkSection::ChunkSection() :
    mLoaded(false),
    mDirty(false)
{
    for (uint32_t i = 0; i < CHUNK_SIZE; ++i)
    {
         mChunkDisplayList.emplace_back(std::make_unique<renderer::DisplayList>());
    }

    mBlocks = new BlockType**[CHUNK_SECTION_SIZE_X];
    for (uint32_t x = 0; x < CHUNK_SECTION_SIZE_X; x++)
    {
        mBlocks[x] = new BlockType*[CHUNK_SECTION_SIZE_Y];
        for (uint32_t y = 0; y < CHUNK_SECTION_SIZE_Y; y++)
        {
            mBlocks[x][y] = new BlockType[CHUNK_SECTION_SIZE_Z];
        }
    }
}

wiicraft::ChunkSection::~ChunkSection()
{
    for (uint32_t x = 0; x < CHUNK_SECTION_SIZE_X; ++x)
    {
        for (uint32_t y = 0; y < CHUNK_SECTION_SIZE_Y; ++y)
        {
            delete[] mBlocks[x][y];
        }
        delete[] mBlocks[x];
    }
    delete[] mBlocks;
}

void wiicraft::ChunkSection::SetTo(const BlockType& blockType)
{
    for (uint32_t x = 0; x < CHUNK_SECTION_SIZE_X; x++)
    {
        for (uint32_t y = 0; y < CHUNK_SECTION_SIZE_Y; y++)
        {
            for (uint32_t z = 0; z < CHUNK_SECTION_SIZE_Z; z++)
            {
                mBlocks[x][y][z] = blockType;
            }
        }
    }
    mLoaded = true;
    mDirty = true;
}

void wiicraft::ChunkSection::Render(renderer::Renderer& renderer, wiicraft::BlockManager& blockmanager)
{
    ASSERT(mLoaded);

    for (uint32_t i = 0; i < CHUNK_SIZE; ++i)
    {
        const math::Vector3f& worldPosition = GetWorldPosition();
        const core::AABB chunkAABB(
        {
           worldPosition.X() + (CHUNK_SIZE * 0.5f),
           worldPosition.Y() + (i * CHUNK_SIZE) + (CHUNK_SIZE * 0.5f),
           worldPosition.Z() + (CHUNK_SIZE * 0.5f)
        },
        {
            CHUNK_SIZE * 0.5f, CHUNK_SIZE * 0.5f, CHUNK_SIZE * 0.5f
        });

        if (renderer.GetCamera()->IsVisible(chunkAABB))
        {
            ++renderer.GetStatistics().ChunksInFrustrum;
            if (mChunkDisplayList[i]->GetBufferSize() == 0 || mDirty)
            {                
                GenerateChunk(i, renderer, blockmanager);
            }
            ASSERT(i <= mChunkDisplayList.size() -1);
            math::Matrix3x4 chunkModelMatrix;
            chunkModelMatrix.SetIdentity();
            chunkModelMatrix.Translate(worldPosition.X(), worldPosition.Y(), worldPosition.Z());
            renderer.LoadModelViewMatrix(renderer.GetCamera()->GetViewMatrix3x4() * chunkModelMatrix);
            mChunkDisplayList[i]->Render();
        }
        else
        {
            ++renderer.GetStatistics().CulledChunks;
            renderer.GetStatistics().ChunkDisplayListSizeMB -= mChunkDisplayList[i]->GetBufferSize() / 1000.0f / 1000.0f;
            mChunkDisplayList[i]->Clear();
        }
    }
    mDirty = false;
}

void wiicraft::ChunkSection::UpdateChunkDisplayList(uint32_t chunkIndex, size_t chunkFaceAmmount,
    const std::unordered_map<wiicraft::BlockType, std::vector<wiicraft::BlockRenderData> > &chunkBlockList,
    renderer::Renderer &renderer,
    wiicraft::BlockManager& blockmanager,
    const renderer::ColorRGBA& color)
{
    if (mChunkDisplayList.size() == 0 || chunkIndex > mChunkDisplayList.size() - 1)
    {
        mChunkDisplayList.emplace_back(std::make_unique<renderer::DisplayList>());
    }

    ASSERT(mChunkDisplayList.size() - 1 >= chunkIndex);
    ASSERT(mChunkDisplayList.size() <= 16);
    renderer::DisplayList& displayList = *mChunkDisplayList[chunkIndex];

    if (displayList.GetBufferSize() > 0)
    {
        renderer.GetStatistics().ChunkDisplayListSizeMB -= displayList.GetBufferSize() / 1000.0f / 1000.0f;
    }

    displayList.Clear();
    if (chunkFaceAmmount == 0)
    {
        return;
    }

    displayList.Begin(400000);

    renderer::VertexFormat chunkBlock(GX_VTXFMT0);
    chunkBlock.AddAttribute({GX_DIRECT, GX_VA_POS, GX_POS_XYZ, GX_F32});
    chunkBlock.AddAttribute({GX_DIRECT, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8});
    chunkBlock.AddAttribute({GX_DIRECT, GX_VA_TEX0, GX_TEX_ST, GX_F32});
    chunkBlock.Bind();

    for (const auto& blockType : chunkBlockList)
    {
        const auto& textureFaceList = blockmanager.GetTextureFacesList(blockType.first);

        for (const auto& texture : textureFaceList)
        {
            texture.first->Bind();

            for (const auto& currentFace : texture.second)
            {
                const std::vector<BlockRenderData>& blockRenderData = blockType.second;
                for (const auto& block : blockRenderData)
                {
                    math::Vector3f vertices[8] =
                    {
                            { (float)block.BlockPosition.x - BlockManager::BLOCK_HALF_SIZE, (float)block.BlockPosition.y + BlockManager::BLOCK_HALF_SIZE, (float)block.BlockPosition.z + BlockManager::BLOCK_HALF_SIZE},// v1
                            { (float)block.BlockPosition.x - BlockManager::BLOCK_HALF_SIZE, (float)block.BlockPosition.y - BlockManager::BLOCK_HALF_SIZE, (float)block.BlockPosition.z + BlockManager::BLOCK_HALF_SIZE}, //v2
                            { (float)block.BlockPosition.x + BlockManager::BLOCK_HALF_SIZE, (float)block.BlockPosition.y - BlockManager::BLOCK_HALF_SIZE, (float)block.BlockPosition.z + BlockManager::BLOCK_HALF_SIZE}, //v3
                            { (float)block.BlockPosition.x + BlockManager::BLOCK_HALF_SIZE, (float)block.BlockPosition.y + BlockManager::BLOCK_HALF_SIZE, (float)block.BlockPosition.z + BlockManager::BLOCK_HALF_SIZE}, // v4
                            { (float)block.BlockPosition.x - BlockManager::BLOCK_HALF_SIZE, (float)block.BlockPosition.y + BlockManager::BLOCK_HALF_SIZE, (float)block.BlockPosition.z - BlockManager::BLOCK_HALF_SIZE}, //v5
                            { (float)block.BlockPosition.x + BlockManager::BLOCK_HALF_SIZE, (float)block.BlockPosition.y + BlockManager::BLOCK_HALF_SIZE, (float)block.BlockPosition.z - BlockManager::BLOCK_HALF_SIZE}, // v6
                            { (float)block.BlockPosition.x + BlockManager::BLOCK_HALF_SIZE, (float)block.BlockPosition.y - BlockManager::BLOCK_HALF_SIZE, (float)block.BlockPosition.z - BlockManager::BLOCK_HALF_SIZE}, // v7
                            { (float)block.BlockPosition.x - BlockManager::BLOCK_HALF_SIZE, (float)block.BlockPosition.y - BlockManager::BLOCK_HALF_SIZE, (float)block.BlockPosition.z - BlockManager::BLOCK_HALF_SIZE} // v8

                    };

                    if ((block.FaceMask & BLOCK_FRONT_FACE) && currentFace == BlockFace::Front)
                    {
                        GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
                            // front side
                            GX_Position3f32(vertices[0].X(), vertices[0].Y(), vertices[0].Z());
                            GX_Color1u32(color.Color());
                            GX_TexCoord2f32(0.0f, 0.0f);

                            GX_Position3f32(vertices[3].X(), vertices[3].Y(), vertices[3].Z());
                            GX_Color1u32(color.Color());
                            GX_TexCoord2f32(1.0f, 0.0f);

                            GX_Position3f32(vertices[2].X(), vertices[2].Y(), vertices[2].Z());
                            GX_Color1u32(color.Color());
                            GX_TexCoord2f32(1.0f, 1.0f);

                            GX_Position3f32(vertices[1].X(), vertices[1].Y(), vertices[1].Z());
                            GX_Color1u32(color.Color());
                            GX_TexCoord2f32(0.0f, 1.0f);
                        GX_End();
                    }

                    if ((block.FaceMask & BLOCK_BACK_FACE) && currentFace == BlockFace::Front)
                    {
                        GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
                            // back side
                            GX_Position3f32(vertices[5].X(), vertices[5].Y(), vertices[5].Z());
                            GX_Color1u32(color.Color());
                            GX_TexCoord2f32(0.0f, 0.0f);

                            GX_Position3f32(vertices[4].X(), vertices[4].Y(), vertices[4].Z());
                            GX_Color1u32(color.Color());
                            GX_TexCoord2f32(1.0f, 0.0f);

                            GX_Position3f32(vertices[7].X(), vertices[7].Y(), vertices[7].Z());
                            GX_Color1u32(color.Color());
                            GX_TexCoord2f32(1.0f, 1.0f);

                            GX_Position3f32(vertices[6].X(), vertices[6].Y(), vertices[6].Z());
                            GX_Color1u32(color.Color());
                            GX_TexCoord2f32(0.0f, 1.0f);
                        GX_End();
                    }

                    if ((block.FaceMask & BLOCK_RIGHT_FACE) && currentFace == BlockFace::Right)
                    {
                        GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
                            // right side
                            GX_Position3f32(vertices[3].X(), vertices[3].Y(), vertices[3].Z());
                            GX_Color1u32(color.Color());
                            GX_TexCoord2f32(0.0f, 0.0f);

                            GX_Position3f32(vertices[5].X(), vertices[5].Y(), vertices[5].Z());
                            GX_Color1u32(color.Color());
                            GX_TexCoord2f32(1.0f, 0.0f);

                            GX_Position3f32(vertices[6].X(), vertices[6].Y(), vertices[6].Z());
                            GX_Color1u32(color.Color());
                            GX_TexCoord2f32(1.0f, 1.0f);

                            GX_Position3f32(vertices[2].X(), vertices[2].Y(), vertices[2].Z());
                            GX_Color1u32(color.Color());
                            GX_TexCoord2f32(0.0f, 1.0f);
                        GX_End();
                    }

                    if ((block.FaceMask & BLOCK_LEFT_FACE) && currentFace == BlockFace::Left)
                    {
                        GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
                            // left side
                            GX_Position3f32(vertices[4].X(), vertices[4].Y(), vertices[4].Z());
                            GX_Color1u32(color.Color());
                            GX_TexCoord2f32(0.0f, 0.0f);

                            GX_Position3f32(vertices[0].X(), vertices[0].Y(), vertices[0].Z());
                            GX_Color1u32(color.Color());
                            GX_TexCoord2f32(1.0f, 0.0f);

                            GX_Position3f32(vertices[1].X(), vertices[1].Y(), vertices[1].Z());
                            GX_Color1u32(color.Color());
                            GX_TexCoord2f32(1.0f, 1.0f);

                            GX_Position3f32(vertices[7].X(), vertices[7].Y(), vertices[7].Z());
                            GX_Color1u32(color.Color());
                            GX_TexCoord2f32(0.0f, 1.0f);
                        GX_End();
                    }

                    if ((block.FaceMask & BLOCK_TOP_FACE) && currentFace == BlockFace::Top)
                    {
                        GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
                            // top side
                            GX_Position3f32(vertices[4].X(), vertices[4].Y(), vertices[4].Z());
                            GX_Color1u32(color.Color());
                            GX_TexCoord2f32(0.0f, 0.0f);

                            GX_Position3f32(vertices[5].X(), vertices[5].Y(), vertices[5].Z());
                            GX_Color1u32(color.Color());
                            GX_TexCoord2f32(1.0f, 0.0f);

                            GX_Position3f32(vertices[3].X(), vertices[3].Y(), vertices[3].Z());
                            GX_Color1u32(color.Color());
                            GX_TexCoord2f32(1.0f, 1.0f);

                            GX_Position3f32(vertices[0].X(), vertices[0].Y(), vertices[0].Z());
                            GX_Color1u32(color.Color());
                            GX_TexCoord2f32(0.0f, 1.0f);
                        GX_End();
                    }

                    if ((block.FaceMask & BLOCK_BOTTOM_FACE) && currentFace == BlockFace::Bottom)
                    {
                        GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
                            // bottom side
                            GX_Position3f32(vertices[6].X(), vertices[6].Y(), vertices[6].Z());
                            GX_Color1u32(color.Color());
                            GX_TexCoord2f32(0.0f, 0.0f);

                            GX_Position3f32(vertices[7].X(), vertices[7].Y(), vertices[7].Z());
                            GX_Color1u32(color.Color());
                            GX_TexCoord2f32(1.0f, 0.0f);

                            GX_Position3f32(vertices[1].X(), vertices[1].Y(), vertices[1].Z());
                            GX_Color1u32(color.Color());
                            GX_TexCoord2f32(1.0f, 1.0f);

                            GX_Position3f32(vertices[2].X(), vertices[2].Y(), vertices[2].Z());
                            GX_Color1u32(color.Color());
                            GX_TexCoord2f32(0.0f, 1.0f);
                        GX_End();
                    }
                }
            }
        }
    }
    displayList.End();
    renderer.GetStatistics().ChunkDisplayListSizeMB += displayList.GetBufferSize() / 1000.0f / 1000.0f;
}

void wiicraft::ChunkSection::GenerateChunk(uint32_t chunkIndex, renderer::Renderer& renderer, wiicraft::BlockManager& blockmanager)
{
    std::unordered_map<BlockType, std::vector<BlockRenderData>> chunkBlockRenderMap;
    size_t chunkFaceAmount = 0;
    for (uint32_t x = 0; x < CHUNK_SIZE; x++)
    {
        for (uint32_t y = 0; y < CHUNK_SIZE; y++)
        {
            for (uint32_t z = 0; z < CHUNK_SIZE; z++)
            {
                const uint32_t chunkSectionY = y + (chunkIndex * CHUNK_SIZE);
                BlockRenderData blockData;
                if (IsBlockVisible(x, chunkSectionY, z, blockData))
                {
                    chunkFaceAmount += blockData.Faces;
                    const BlockType& blockType = mBlocks[x][chunkSectionY][z];
                    const auto& blockRenderMapIterator = chunkBlockRenderMap.find(blockType);
                    if (blockRenderMapIterator == chunkBlockRenderMap.end())
                    {
                        chunkBlockRenderMap.insert(std::pair<BlockType, std::vector<BlockRenderData>>(blockType, {blockData}));
                    }
                    else
                    {
                        blockRenderMapIterator->second.emplace_back(blockData);
                    }
                }
            }
        }
    }
    UpdateChunkDisplayList(chunkIndex, chunkFaceAmount, chunkBlockRenderMap, renderer, blockmanager, renderer::ColorRGBA::WHITE);
}

bool wiicraft::ChunkSection::IsBlockVisible(uint32_t x, uint32_t y, uint32_t z, wiicraft::BlockRenderData &blockRenderVO) const
{
    bool bIsAir = mBlocks[x][y][z] == BlockType::AIR;

    if (bIsAir)
    {
        return false;
    }

    BlockType top = y == CHUNK_SECTION_SIZE_Y - 1 ? BlockType::AIR : mBlocks[x][y + 1][z];
    BlockType bottom = y == 0 ? BlockType::AIR : mBlocks[x][y - 1][z];
    BlockType north = z == CHUNK_SECTION_SIZE_Z - 1 ? BlockType::AIR : mBlocks[x][y][z + 1];
    BlockType south = z == 0 ? BlockType::AIR : mBlocks[x][y][z - 1];
    BlockType east = x == CHUNK_SECTION_SIZE_X - 1 ? BlockType::AIR : mBlocks[x + 1][y][z];
    BlockType west = x == 0 ? BlockType::AIR : mBlocks[x - 1][y][z];

    if (top == BlockType::AIR)
    {
        blockRenderVO.FaceMask |= BLOCK_TOP_FACE;
        blockRenderVO.Faces++;
    }
    if (bottom == BlockType::AIR)
    {
        blockRenderVO.FaceMask |= BLOCK_BOTTOM_FACE;
        blockRenderVO.Faces++;
    }
    if (north == BlockType::AIR)
    {
        blockRenderVO.FaceMask |= BLOCK_FRONT_FACE;
        blockRenderVO.Faces++;
    }
    if (south == BlockType::AIR)
    {
        blockRenderVO.FaceMask |= BLOCK_BACK_FACE;
        blockRenderVO.Faces++;
    }
    if (east == BlockType::AIR)
    {
        blockRenderVO.FaceMask |= BLOCK_RIGHT_FACE;
        blockRenderVO.Faces++;
    }
    if (west == BlockType::AIR)
    {
        blockRenderVO.FaceMask |= BLOCK_LEFT_FACE;
        blockRenderVO.Faces++;
    }

    if (blockRenderVO.Faces > 0)
    {
        blockRenderVO.BlockPosition = { x, y, z };
        return true;
    }

    return false;
}

wiicraft::ChunkPosition wiicraft::ChunkSection::WorldPositionToChunkPosition(const math::Vector3f& worldPosition)
{
    ChunkPosition pos;
    pos.first = static_cast<int32_t>(std::floor(worldPosition.X() / CHUNK_SIZE));
    pos.second = static_cast<int32_t>(std::floor(worldPosition.Z() / CHUNK_SIZE));
    return pos;
}

math::Vector3f wiicraft::ChunkSection::ChunkPositionToWorldPosition(const wiicraft::ChunkPosition &chunkPosition)
{
    return {static_cast<float>(chunkPosition.first * static_cast<int32_t>(CHUNK_SIZE)), 0.0f,
                static_cast<float>(chunkPosition.second * static_cast<int32_t>(CHUNK_SIZE))};
}

math::Vector3f wiicraft::ChunkSection::WorldPositionToBlockPosition(const math::Vector3f &worldPosition, const wiicraft::ChunkPosition &chunkPosition)
{
    const math::Vector3f& chunkWorldPos = ChunkSection::ChunkPositionToWorldPosition(chunkPosition);
    BlockPosition blockPosition;
    blockPosition.x = static_cast<uint32_t>(std::round(math::Mod<float>(worldPosition.X(), CHUNK_SECTION_SIZE_X)));
    blockPosition.y = static_cast<uint32_t>(std::round(math::Mod<float>(worldPosition.Y(), CHUNK_SECTION_SIZE_Y)));
    blockPosition.z = static_cast<uint32_t>(std::round(math::Mod<float>(worldPosition.Z(), CHUNK_SECTION_SIZE_Z)));
    return {chunkWorldPos.X() + blockPosition.x, static_cast<float>(blockPosition.y), chunkWorldPos.Z() + blockPosition.z};
}

wiicraft::BlockPosition wiicraft::ChunkSection::BlockWorldPositionToLocalChunkPosition(const math::Vector3f &worldPosition)
{
    BlockPosition blockPosition;
    blockPosition.x = static_cast<uint32_t>(std::round(math::Mod<float>(worldPosition.X(), CHUNK_SECTION_SIZE_X)));
    blockPosition.y = static_cast<uint32_t>(std::round(math::Mod<float>(worldPosition.Y(), CHUNK_SECTION_SIZE_Y)));
    blockPosition.z = static_cast<uint32_t>(std::round(math::Mod<float>(worldPosition.Z(), CHUNK_SECTION_SIZE_Z)));
    return blockPosition;
}

math::Vector3f wiicraft::ChunkSection::WorldPositionToBlockPosition(const math::Vector3f &worldPosition) const
{
    BlockPosition blockPosition;
    blockPosition.x = static_cast<uint32_t>(std::round(math::Mod<float>(worldPosition.X(), CHUNK_SECTION_SIZE_X)));
    blockPosition.y = static_cast<uint32_t>(std::round(math::Mod<float>(worldPosition.Y(), CHUNK_SECTION_SIZE_Y)));
    blockPosition.z = static_cast<uint32_t>(std::round(math::Mod<float>(worldPosition.Z(), CHUNK_SECTION_SIZE_Z)));
    return {GetWorldPosition().X() + blockPosition.x, static_cast<float>(blockPosition.y), GetWorldPosition().Z() + blockPosition.z};
}

std::pair<math::Vector3f, wiicraft::BlockType> wiicraft::ChunkSection::GetBlockTypeByWorldPosition(const math::Vector3f &worldPosition) const
{
    BlockPosition blockPosition;
    blockPosition.x = static_cast<uint32_t>(std::round(math::Mod<float>(worldPosition.X(), CHUNK_SECTION_SIZE_X)));
    blockPosition.y = static_cast<uint32_t>(std::round(math::Mod<float>(worldPosition.Y(), CHUNK_SECTION_SIZE_Y)));
    blockPosition.z = static_cast<uint32_t>(std::round(math::Mod<float>(worldPosition.Z(), CHUNK_SECTION_SIZE_Z)));
    return std::make_pair(WorldPositionToBlockPosition(worldPosition),
                          mBlocks[blockPosition.x][blockPosition.y][blockPosition.z]);
}

void wiicraft::ChunkSection::SetBlock(const BlockPosition& position, const wiicraft::BlockType &blockType)
{
    mBlocks[position.x][position.y][position.z] = blockType;
    mChunkDisplayList[position.y / CHUNK_SIZE]->Clear();
}


