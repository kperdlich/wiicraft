#pragma once

#include "wiidefines.h"

namespace renderer {
class DisplayList
{
public:
    DisplayList(const size_t sizeOfDisplayList);
    ~DisplayList();
    DisplayList(const DisplayList&) = delete;
    DisplayList(DisplayList&&) = delete;
    void operator=(const DisplayList&) = delete;
    void operator=(DisplayList&&) = delete;
    void Render();
    void End();
    void Clear();
    inline uint32_t GetBufferSize() const;

private:
    void* mDispList;
    uint32_t mBufferSize;
};

inline uint32_t DisplayList::GetBufferSize() const
{
    return mBufferSize;
}
}



