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

#include <unordered_map>
#include <vector>
#include "texture2d.h"
#include "image2d.h"
#include "chunksection.h"
#include "staticmesh.h"

namespace wiicraft {

enum class BlockFace : uint8_t
{
    Left,
    Right,
    Front,
    Back,
    Top,
    Bottom
};


class BlockManager {
public:
    static constexpr float BLOCK_HALF_SIZE = 0.5f;
    static constexpr float BLOCK_SIZE = BLOCK_HALF_SIZE * 2.0f;
    BlockManager();
    ~BlockManager();
	BlockManager(const BlockManager&) = delete;
	BlockManager(BlockManager&&) = delete;
	void operator=(const BlockManager&) = delete;
	void operator=(BlockManager&&) = delete;		

    const std::unordered_map<renderer::Texture2D*, std::vector<BlockFace>>& GetTextureFacesList(const BlockType& blockType) const;
    void GenerateMultiTexturedBlockMesh(BlockType blockType, renderer::DisplayList& displayList);

private:
    void LoadBlocks();
    void UnloadBlocks();
    renderer::Texture2D *LoadImageAndAddToTextureList(const renderer::Image2D *image);
private:    
    std::unordered_map<BlockType, std::unordered_map<renderer::Texture2D*, std::vector<BlockFace>>> m_blocks;
    std::vector<const renderer::Texture2D*> m_textures;
    std::vector<const renderer::Image2D*> m_images;

};
}
