#pragma once

#include "stdint.h"

namespace wiicraft
{

    struct CompressedChunkData
    {
        int32_t m_X;
        int32_t m_Z;
        bool m_bGroundUpCon;
        uint16_t m_PrimaryBitMap;
        uint16_t m_AddBitMap;
        int32_t m_CompressedSize;
        unsigned char* m_CompressedData;
    };

} // namespace wiicraft
