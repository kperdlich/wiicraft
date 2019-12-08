#pragma once

#include <stdint.h>

namespace renderer {

struct Statistics
{
    uint8_t FPS = 0;
    uint32_t ChunksInFrustrum = 0;
    uint32_t CulledChunks = 0;
    float ChunkDisplayListSizeMB = 0;
};

}


