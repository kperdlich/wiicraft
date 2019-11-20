#pragma once

#include <iostream>
#include "core.h"
#include <memory>
#include <stddef.h>
#include <stdint.h>

namespace renderer {

class IndexBuffer
{
public:
    IndexBuffer(const std::initializer_list<uint16_t>& indecies);
    ~IndexBuffer() = default;
    IndexBuffer(const IndexBuffer&) = delete;
    IndexBuffer(IndexBuffer&&) = delete;
    IndexBuffer& operator=(const IndexBuffer&) = delete;
    IndexBuffer& operator=(IndexBuffer&&) = delete;

    inline uint16_t operator [](uint32_t index) const;
    inline uint16_t GetIndexAt(uint32_t index) const;
    inline size_t GetElementCount() const;

private:
    std::unique_ptr<uint16_t[]> mBuffer;
    size_t mElementCount;
};

inline uint16_t IndexBuffer::operator [](uint32_t index) const
{
    ASSERT(index >= 0 && index < mElementCount);
    return mBuffer[index];
}

inline uint16_t IndexBuffer::GetIndexAt(uint32_t index) const
{
    ASSERT(index >= 0 && index < mElementCount);
    return mBuffer[index];
}

inline size_t IndexBuffer::GetElementCount() const
{
    return mElementCount;
}

}
