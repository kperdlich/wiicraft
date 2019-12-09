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

#include "BlockManager.h"
#include "core.h"
#include "image2d.h"
#include "Dirt_tpl.h"
#include "Grass_tpl.h"
#include "Grass_Side_tpl.h"
#include "Stone_tpl.h"
#include "Wood_tpl.h"
#include "Leaf_tpl.h"
#include "Tree_tpl.h"

wiicraft::BlockManager::BlockManager()
{
    LoadBlocks();
}

wiicraft::BlockManager::~BlockManager()
{
    UnloadBlocks();
}

void wiicraft::BlockManager::LoadBlocks()
{
    renderer::Texture2D* pDirtTexture = LoadImageAndAddToTextureList(new renderer::Image2D(Dirt_tpl, Dirt_tpl_size));
    renderer::Texture2D* pGrassTexture = LoadImageAndAddToTextureList(new renderer::Image2D(Grass_tpl, Grass_tpl_size));
    renderer::Texture2D* pGrassSideTexture = LoadImageAndAddToTextureList(new renderer::Image2D(Grass_Side_tpl, Grass_Side_tpl_size));
    renderer::Texture2D* pStoneTexture = LoadImageAndAddToTextureList(new renderer::Image2D(Stone_tpl, Stone_tpl_size));
    renderer::Texture2D* pWoodTexture = LoadImageAndAddToTextureList(new renderer::Image2D(Wood_tpl, Wood_tpl_size));
    renderer::Texture2D* pLeafTexture = LoadImageAndAddToTextureList(new renderer::Image2D(Leaf_tpl, Leaf_tpl_size));
    renderer::Texture2D* pTreeTexture = LoadImageAndAddToTextureList(new renderer::Image2D(Tree_tpl, Tree_tpl_size));

    std::unordered_map<renderer::Texture2D*, std::vector<BlockFace>> dirtTextureMap =
	{
		{
		  pDirtTexture,
		  {
              BlockFace::Left,
              BlockFace::Right,
              BlockFace::Front,
              BlockFace::Back,
              BlockFace::Top,
              BlockFace::Bottom,
		  }
		}
	};

    std::unordered_map<renderer::Texture2D*, std::vector<BlockFace>> stoneTextureMap =
	{
		{
		  pStoneTexture,
		  {
              BlockFace::Left,
              BlockFace::Right,
              BlockFace::Front,
              BlockFace::Back,
              BlockFace::Top,
              BlockFace::Bottom,
		  }
		}
	};

    std::unordered_map<renderer::Texture2D*, std::vector<BlockFace>> leafTextureMap =
	{
		{
		  pLeafTexture,
		  {
              BlockFace::Left,
              BlockFace::Right,
              BlockFace::Front,
              BlockFace::Back,
              BlockFace::Top,
              BlockFace::Bottom,
		  }
		}
	};

    std::unordered_map<renderer::Texture2D*, std::vector<BlockFace>> grassTextureMap =
	{
		{
			pGrassTexture,
			{
                  BlockFace::Top
			}
		},
		{
			pDirtTexture,
			{
                  BlockFace::Bottom
			}
		},
		{
			pGrassSideTexture,
			{
                BlockFace::Left,
                BlockFace::Right,
                BlockFace::Front,
                BlockFace::Back
			}
		}
	};

    std::unordered_map<renderer::Texture2D*, std::vector<BlockFace>> woodTextureMap =
	{
		{
			pTreeTexture,
			{
                  BlockFace::Top,
                  BlockFace::Bottom
			}
		},
		{
			pWoodTexture,
			{
                BlockFace::Left,
                BlockFace::Right,
                BlockFace::Front,
                BlockFace::Back
			}
		}
	};

    m_blocks.insert(std::pair< BlockType, std::unordered_map<renderer::Texture2D*, std::vector<BlockFace>>>(BlockType::AIR, {})); // no texture for air!
    m_blocks.insert(std::pair< BlockType, std::unordered_map<renderer::Texture2D*, std::vector<BlockFace>>>(BlockType::DIRT, dirtTextureMap));
    m_blocks.insert(std::pair< BlockType, std::unordered_map<renderer::Texture2D*, std::vector<BlockFace>>>(BlockType::GRASS,  grassTextureMap));
    m_blocks.insert(std::pair< BlockType, std::unordered_map<renderer::Texture2D*, std::vector<BlockFace>>>(BlockType::STONE, stoneTextureMap));
    m_blocks.insert(std::pair< BlockType, std::unordered_map<renderer::Texture2D*, std::vector<BlockFace>>>(BlockType::WOOD, woodTextureMap));
    m_blocks.insert(std::pair< BlockType, std::unordered_map<renderer::Texture2D*, std::vector<BlockFace>>>(BlockType::LEAF, leafTextureMap));
}

void wiicraft::BlockManager::UnloadBlocks()
{
	m_blocks.clear();
    m_textures.clear();

    for (auto image : m_images)
	{
        delete image;
	}
    m_images.clear();
}

const std::unordered_map<renderer::Texture2D *, std::vector<wiicraft::BlockFace>> &wiicraft::BlockManager::GetTextureFacesList(
        const BlockType &blockType) const
{
    const auto& blockIterator = m_blocks.find(blockType);
    if (blockIterator != m_blocks.end())
    {
        return blockIterator->second;
    }

   ASSERT(false);
}

renderer::Texture2D* wiicraft::BlockManager::LoadImageAndAddToTextureList(const renderer::Image2D* image)
{
    renderer::Texture2D* texture = new renderer::Texture2D(*image);
    m_images.push_back(image);
    m_textures.push_back(texture);
	return texture;
}
