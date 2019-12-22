#include "BlockManager.h"
#include "vertexformat.h"
#include "renderer.h"
#include "core.h"
#include "image2d.h"
#include "test_tpl.h"

wiicraft::BlockManager::BlockManager()
{
    mBlockSheetImage = std::make_unique<renderer::Image2D>(test_tpl, test_tpl_size);
    mBlockSheetTexture = std::make_unique<renderer::Texture2D>(*mBlockSheetImage);

    RegisterBlock(std::make_shared<Block>(BlockType::STONE, 0));
    RegisterBlock(std::make_shared<BlockGrass>(BlockType::GRASS, 3));
    RegisterBlock(std::make_shared<Block>(BlockType::DIRT, 2));
    RegisterBlock(std::make_shared<Block>(BlockType::COBBLESTONE, 16));
    RegisterBlock(std::make_shared<Block>(BlockType::PLANKS, 5));
    RegisterBlock(std::make_shared<Block>(BlockType::SAPLINGS, 15));
    RegisterBlock(std::make_shared<Block>(BlockType::BEDROCK, 17));
    RegisterBlock(std::make_shared<Block>(BlockType::STATIONARY_WATER, 12 * 16 + 13));
    RegisterBlock(std::make_shared<Block>(BlockType::LAVA, 14 * 16 + 13));
    RegisterBlock(std::make_shared<Block>(BlockType::STATIONARY_LAVA, 14 * 16 + 13));
    RegisterBlock(std::make_shared<Block>(BlockType::WATER, 12 * 16 + 13));
    RegisterBlock(std::make_shared<Block>(BlockType::GLASS, 49));
    RegisterBlock(std::make_shared<Block>(BlockType::SAND, 18));
    RegisterBlock(std::make_shared<Block>(BlockType::WOOD, 20));
    RegisterBlock(std::make_shared<Block>(BlockType::BRICKS, 7));
    RegisterBlock(std::make_shared<BlockLeaf>(BlockType::LEAF, 53));
    RegisterBlock(std::make_shared<Block>(BlockType::TNT, 8));
}

void wiicraft::BlockManager::BindTextureSheet(uint8_t slot)
{
    mBlockSheetTexture->Bind(slot);
}


std::shared_ptr<wiicraft::Block> wiicraft::BlockManager::GetBlock(wiicraft::BlockType type)
{
    auto findIt = mBlockList.find(type);
    if (findIt != mBlockList.end())
    {
        return findIt->second;
    }
    ASSERT_TEXT(false, "BlockID: %d not implemented!", static_cast<uint8_t>(type));
    return nullptr;
}

void wiicraft::BlockManager::RegisterBlock(std::shared_ptr<wiicraft::Block> block)
{
    mBlockList[block->GetBlockType()] = block;
}

std::array<float, 8> wiicraft::BlockManager::GetUVTextureCoordinates(int32_t index) const
{
    const int32_t width = mBlockSheetImage->Width();
    const int32_t height = mBlockSheetImage->Height();
    const int32_t rows = width / TILE_HEIGHT;
    const int32_t columns = height / TILE_WIDTH;
    const int32_t x = (index % columns) * TILE_WIDTH;
    const int32_t y = (index / rows) * TILE_HEIGHT;

    const float sTopLeft = static_cast<float>(x) / width;
    const float tTopLeft = static_cast<float>(y) / height;

    const float sTopRight = static_cast<float>(x + TILE_WIDTH) / width;
    const float tTopRight = tTopLeft;

    const float sBottomRight = sTopRight;
    const float tBottomRight = static_cast<float>(y + TILE_HEIGHT) / height;

    const float sBottomLeft = sTopLeft;
    const float tBottomLeft = tBottomRight;

    std::array<float, 8> coords;
    coords[0] = sTopLeft;
    coords[1] = tTopLeft;

    coords[2] = sTopRight;
    coords[3] = tTopRight;

    coords[4] = sBottomRight;
    coords[5] = tBottomRight;

    coords[6] = sBottomLeft;
    coords[7] = tBottomLeft;

    return coords;
}


void wiicraft::BlockManager::GenerateMultiTexturedBlockMesh(wiicraft::BlockType blockType, renderer::DisplayList &displayList)
{
    auto findIt = mBlockList.find(blockType);
    ASSERT(findIt != mBlockList.end());

    displayList.Clear();
    displayList.Begin(4000);

    BindTextureSheet();

    renderer::VertexFormat chunkBlock(GX_VTXFMT0);
    chunkBlock.AddAttribute({GX_DIRECT, GX_VA_POS, GX_POS_XYZ, GX_F32});
    chunkBlock.AddAttribute({GX_DIRECT, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8});
    chunkBlock.AddAttribute({GX_DIRECT, GX_VA_TEX0, GX_TEX_ST, GX_F32});
    chunkBlock.Bind();


    math::Vector3f blockPosition = {0.0f, 0.0f, 0.0f};
    math::Vector3f vertices[8] =
        {
                { blockPosition.X() - BLOCK_HALF_SIZE, blockPosition.Y() + BLOCK_HALF_SIZE, blockPosition.Z() + BLOCK_HALF_SIZE},// v1
                { blockPosition.X() - BLOCK_HALF_SIZE, blockPosition.Y() - BLOCK_HALF_SIZE, blockPosition.Z() + BLOCK_HALF_SIZE}, //v2
                { blockPosition.X() + BLOCK_HALF_SIZE, blockPosition.Y() - BLOCK_HALF_SIZE, blockPosition.Z() + BLOCK_HALF_SIZE}, //v3
                { blockPosition.X() + BLOCK_HALF_SIZE, blockPosition.Y() + BLOCK_HALF_SIZE, blockPosition.Z() + BLOCK_HALF_SIZE}, // v4
                { blockPosition.X() - BLOCK_HALF_SIZE, blockPosition.Y() + BLOCK_HALF_SIZE, blockPosition.Z() - BLOCK_HALF_SIZE}, //v5
                { blockPosition.X() + BLOCK_HALF_SIZE, blockPosition.Y() + BLOCK_HALF_SIZE, blockPosition.Z() - BLOCK_HALF_SIZE}, // v6
                { blockPosition.X() + BLOCK_HALF_SIZE, blockPosition.Y() - BLOCK_HALF_SIZE, blockPosition.Z() - BLOCK_HALF_SIZE}, // v7
                { blockPosition.X() - BLOCK_HALF_SIZE, blockPosition.Y() - BLOCK_HALF_SIZE, blockPosition.Z() - BLOCK_HALF_SIZE} // v8
        };



        for (uint8_t faceIndex = 0; faceIndex < 6; ++faceIndex)
        {
            BlockFace currentFace = static_cast<BlockFace>(faceIndex);
            int32_t textureIndex = findIt->second->GetTextureIndexForSide(currentFace);
            const renderer::ColorRGBA& color = findIt->second->GetColorForSide(currentFace);
            const std::array<float, 8> uvs = GetUVTextureCoordinates(textureIndex);

            if (currentFace == BlockFace::Front)
            {
                GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
                    // front side
                    GX_Position3f32(vertices[0].X(), vertices[0].Y(), vertices[0].Z());
                    GX_Color1u32(color.Color());
                    GX_TexCoord2f32(uvs[0], uvs[1]);

                    GX_Position3f32(vertices[3].X(), vertices[3].Y(), vertices[3].Z());
                    GX_Color1u32(color.Color());
                    GX_TexCoord2f32(uvs[2], uvs[3]);

                    GX_Position3f32(vertices[2].X(), vertices[2].Y(), vertices[2].Z());
                    GX_Color1u32(color.Color());
                    GX_TexCoord2f32(uvs[4], uvs[5]);

                    GX_Position3f32(vertices[1].X(), vertices[1].Y(), vertices[1].Z());
                    GX_Color1u32(color.Color());
                    GX_TexCoord2f32(uvs[6], uvs[7]);
                GX_End();
            }

            if (currentFace == BlockFace::Front)
            {
                GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
                    // back side
                    GX_Position3f32(vertices[5].X(), vertices[5].Y(), vertices[5].Z());
                    GX_Color1u32(color.Color());
                    GX_TexCoord2f32(uvs[0], uvs[1]);

                    GX_Position3f32(vertices[4].X(), vertices[4].Y(), vertices[4].Z());
                    GX_Color1u32(color.Color());
                    GX_TexCoord2f32(uvs[2], uvs[3]);

                    GX_Position3f32(vertices[7].X(), vertices[7].Y(), vertices[7].Z());
                    GX_Color1u32(color.Color());
                    GX_TexCoord2f32(uvs[4], uvs[5]);

                    GX_Position3f32(vertices[6].X(), vertices[6].Y(), vertices[6].Z());
                    GX_Color1u32(color.Color());
                    GX_TexCoord2f32(uvs[6], uvs[7]);
                GX_End();
            }

            if (currentFace == BlockFace::Right)
            {
                GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
                    // right side
                    GX_Position3f32(vertices[3].X(), vertices[3].Y(), vertices[3].Z());
                    GX_Color1u32(color.Color());
                    GX_TexCoord2f32(uvs[0], uvs[1]);

                    GX_Position3f32(vertices[5].X(), vertices[5].Y(), vertices[5].Z());
                    GX_Color1u32(color.Color());
                    GX_TexCoord2f32(uvs[2], uvs[3]);

                    GX_Position3f32(vertices[6].X(), vertices[6].Y(), vertices[6].Z());
                    GX_Color1u32(color.Color());
                    GX_TexCoord2f32(uvs[4], uvs[5]);

                    GX_Position3f32(vertices[2].X(), vertices[2].Y(), vertices[2].Z());
                    GX_Color1u32(color.Color());
                    GX_TexCoord2f32(uvs[6], uvs[7]);
                GX_End();
            }

            if (currentFace == BlockFace::Left)
            {
                GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
                    // left side
                    GX_Position3f32(vertices[4].X(), vertices[4].Y(), vertices[4].Z());
                    GX_Color1u32(color.Color());
                    GX_TexCoord2f32(uvs[0], uvs[1]);

                    GX_Position3f32(vertices[0].X(), vertices[0].Y(), vertices[0].Z());
                    GX_Color1u32(color.Color());
                    GX_TexCoord2f32(uvs[2], uvs[3]);

                    GX_Position3f32(vertices[1].X(), vertices[1].Y(), vertices[1].Z());
                    GX_Color1u32(color.Color());
                    GX_TexCoord2f32(uvs[4], uvs[5]);

                    GX_Position3f32(vertices[7].X(), vertices[7].Y(), vertices[7].Z());
                    GX_Color1u32(color.Color());
                    GX_TexCoord2f32(uvs[6], uvs[7]);
                GX_End();
            }

            if (currentFace == BlockFace::Top)
            {
                GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
                    // top side
                    GX_Position3f32(vertices[4].X(), vertices[4].Y(), vertices[4].Z());
                    GX_Color1u32(color.Color());
                    GX_TexCoord2f32(uvs[0], uvs[1]);

                    GX_Position3f32(vertices[5].X(), vertices[5].Y(), vertices[5].Z());
                    GX_Color1u32(color.Color());
                    GX_TexCoord2f32(uvs[2], uvs[3]);

                    GX_Position3f32(vertices[3].X(), vertices[3].Y(), vertices[3].Z());
                    GX_Color1u32(color.Color());
                    GX_TexCoord2f32(uvs[4], uvs[5]);

                    GX_Position3f32(vertices[0].X(), vertices[0].Y(), vertices[0].Z());
                    GX_Color1u32(color.Color());
                    GX_TexCoord2f32(uvs[6], uvs[7]);
                GX_End();
            }

            if (currentFace == BlockFace::Bottom)
            {
                GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
                    // bottom side
                    GX_Position3f32(vertices[6].X(), vertices[6].Y(), vertices[6].Z());
                    GX_Color1u32(color.Color());
                    GX_TexCoord2f32(uvs[0], uvs[1]);

                    GX_Position3f32(vertices[7].X(), vertices[7].Y(), vertices[7].Z());
                    GX_Color1u32(color.Color());
                    GX_TexCoord2f32(uvs[2], uvs[3]);

                    GX_Position3f32(vertices[1].X(), vertices[1].Y(), vertices[1].Z());
                    GX_Color1u32(color.Color());
                    GX_TexCoord2f32(uvs[4], uvs[5]);

                    GX_Position3f32(vertices[2].X(), vertices[2].Y(), vertices[2].Z());
                    GX_Color1u32(color.Color());
                    GX_TexCoord2f32(uvs[6], uvs[7]);
                GX_End();
            }
        }

    displayList.End();
}


