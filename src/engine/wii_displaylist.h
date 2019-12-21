#pragma once

#include "wii_defines.h"

namespace renderer {
class DisplayList
{
public:
    DisplayList();
    explicit DisplayList(const size_t sizeOfDisplayList);
    ~DisplayList();
    DisplayList(const DisplayList&) = delete;
    DisplayList(DisplayList&&) = delete;
    void operator=(const DisplayList&) = delete;
    void operator=(DisplayList&&) = delete;
    void Render();
    void Begin(const size_t bufferSize);
    void End();
    void Clear();
    inline uint32_t GetBufferSize() const;
    inline bool IsDirty() const;

private:
    void* mDispList;
    uint32_t mBufferSize;
};

inline uint32_t DisplayList::GetBufferSize() const
{
    return mBufferSize;
}

inline bool DisplayList::IsDirty() const
{
    return mBufferSize == 0 || !mDispList;
}

}



