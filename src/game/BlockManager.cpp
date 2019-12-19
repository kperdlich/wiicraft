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

void wiicraft::BlockManager::GenerateMultiTexturedBlockMesh(wiicraft::BlockType blockType, renderer::DisplayList &displayList)
{
    auto findIt = m_blocks.find(blockType);
    ASSERT(findIt != m_blocks.end());

    displayList.Clear();
    displayList.Begin(4000);

    renderer::VertexFormat chunkBlock(GX_VTXFMT0);
    chunkBlock.AddAttribute({GX_DIRECT, GX_VA_POS, GX_POS_XYZ, GX_F32});
    chunkBlock.AddAttribute({GX_DIRECT, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8});
    chunkBlock.AddAttribute({GX_DIRECT, GX_VA_TEX0, GX_TEX_ST, GX_F32});
    chunkBlock.Bind();


    renderer::ColorRGBA color = renderer::ColorRGBA::WHITE;
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

    for (const auto& texture : findIt->second)
    {
        texture.first->Bind();

        for (const auto& currentFace : texture.second)
        {
                if (currentFace == BlockFace::Front)
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

                if (currentFace == BlockFace::Front)
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

                if (currentFace == BlockFace::Right)
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

                if (currentFace == BlockFace::Left)
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

                if (currentFace == BlockFace::Top)
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

                if (currentFace == BlockFace::Bottom)
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

    displayList.End();
}


