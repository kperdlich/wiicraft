/***
 *
 * Copyright (C) 2016 DaeFennek
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***/

#pragma once

#include "Block.h"
#include "Image2d.h"
#include "Texture2d.h"
#include "WiiDisplayList.h"
#include <array>
#include <memory>
#include <unordered_map>

namespace wiicraft
{

    class BlockManager
    {
    public:
        static constexpr float BLOCK_HALF_SIZE = 0.5f;
        static constexpr float BLOCK_SIZE = BLOCK_HALF_SIZE * 2.0f;
        static constexpr uint32_t TILE_HEIGHT = 32;
        static constexpr uint32_t TILE_WIDTH = 32;

        BlockManager();
        BlockManager(const BlockManager&) = delete;
        BlockManager(BlockManager&&) = delete;
        void operator=(const BlockManager&) = delete;
        void operator=(BlockManager&&) = delete;

        void BindTextureSheet(uint8_t slot = 0);
        std::shared_ptr<Block> GetBlock(BlockType type);
        void GenerateMultiTexturedBlockMesh(BlockType blockType, renderer::DisplayList& displayList);
        std::array<float, 8> GetUVTextureCoordinates(int32_t index) const;

        inline int32_t GetSheetWidth() const;
        inline int32_t GetSheetHeight() const;

    private:
        void RegisterBlock(std::shared_ptr<Block> block);

    private:
        std::unordered_map<BlockType, std::shared_ptr<Block>> mBlockList;
        std::unique_ptr<renderer::Image2D> mBlockSheetImage;
        std::unique_ptr<renderer::Texture2D> mBlockSheetTexture;
    };

    inline int32_t BlockManager::GetSheetWidth() const
    {
        return mBlockSheetImage->Width();
    }

    inline int32_t BlockManager::GetSheetHeight() const
    {
        return mBlockSheetImage->Height();
    }
} // namespace wiicraft
