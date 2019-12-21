#pragma once

#include "wii_defines.h"

namespace utils {
class Clock
{
public:
    Clock() = default;
    ~Clock() = default;
	Clock(const Clock&) = delete;
	Clock(Clock&&) = delete;
    Clock& operator=(const Clock&) = delete;
    Clock& operator=(Clock&&) = delete;

    inline void Start();
    inline uint64_t ElapsedMilliseconds() const;
private:
    uint64_t mMilliSecs;
};

inline void Clock::Start()
{
    mMilliSecs = ticks_to_millisecs(gettime());
}

inline uint64_t Clock::ElapsedMilliseconds() const
{
    return ticks_to_millisecs(gettime()) - mMilliSecs;
}

}
