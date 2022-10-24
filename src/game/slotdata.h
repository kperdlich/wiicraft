#pragma once

#include <stdint.h>

namespace wiicraft
{

    struct SlotData
    {
        int16_t ItemID = -1;
        int16_t ItemDamage = 0;
        int8_t ItemCount = 0;
    };

} // namespace wiicraft
