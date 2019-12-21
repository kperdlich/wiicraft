#include "time_helper.h"
#include "wii_defines.h"

uint64_t core::GetMsCount()
{
    return ticks_to_millisecs(gettime());
}

