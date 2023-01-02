#pragma once

#include "ColorRGBA.h"
#include <stdint.h>

namespace wiicraft
{

    enum class BlockType : int8_t
    {
        AIR = 0,
        STONE = 1,
        GRASS = 2,
        DIRT = 3,
        COBBLESTONE = 4,
        PLANKS = 5,
        SAPLINGS = 6,
        BEDROCK = 7,
        WATER = 8,
        STATIONARY_WATER = 9,
        LAVA = 10,
        STATIONARY_LAVA = 11,
        SAND = 12,
        GLASS = 14,
        WOOD = 17,
        LEAF = 18,
        BRICKS = 45,
        TNT = 46,
    };

    enum class BlockFace : uint8_t
    {
        Left,
        Right,
        Front,
        Back,
        Top,
        Bottom
    };

    class Block
    {
    public:
        Block(BlockType id, uint32_t textureIndex);
        virtual ~Block() = default;
        virtual int32_t GetTextureIndexForSide(BlockFace side);
        virtual renderer::ColorRGBA GetColorForSide(BlockFace side);
        inline BlockType GetBlockType() const;

    private:
        uint32_t mBlockTextureIndex;
        BlockType mBlockId;
    };

    inline BlockType Block::GetBlockType() const
    {
        return mBlockId;
    }

    class BlockGrass : public Block
    {
    public:
        BlockGrass(BlockType id, uint32_t textureIndex)
            : Block(id, textureIndex)
        {
        }

        virtual int32_t GetTextureIndexForSide(BlockFace side) override
        {
            if (side == BlockFace::Top)
            {
                return 0;
            }
            return side == BlockFace::Bottom ? 2 : 3;
        }

        virtual renderer::ColorRGBA GetColorForSide(BlockFace side) override
        {
            if (side == BlockFace::Top)
            {
                return { 0x73, 0xb3, 0x49, 0xFF };
            }
            return renderer::ColorRGBA::WHITE;
        }
    };

    class BlockLeaf : public Block
    {
    public:
        BlockLeaf(BlockType id, uint32_t textureIndex)
            : Block(id, textureIndex)
        {
        }

        virtual renderer::ColorRGBA GetColorForSide(BlockFace side) override
        {
            return { 0x48, 0x70, 0x40, 0xFF };
        }
    };

} // namespace wiicraft
