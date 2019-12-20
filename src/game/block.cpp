#include "block.h"

wiicraft::Block::Block(BlockType id, uint32_t textureIndex)
    : mBlockTextureIndex(textureIndex),
      mBlockId(id)
{
}

int32_t wiicraft::Block::GetTextureIndexForSide(wiicraft::BlockFace side)
{
    return mBlockTextureIndex;
}

renderer::ColorRGBA wiicraft::Block::GetColorForSide(wiicraft::BlockFace side)
{
    return renderer::ColorRGBA::WHITE;
}
