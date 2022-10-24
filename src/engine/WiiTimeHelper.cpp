#include "TimeHelper.h"
#include "WiiDefines.h"

uint64_t core::GetMsCount()
{
    return ticks_to_millisecs(gettime());
}
