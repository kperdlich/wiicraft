#pragma once

#include <iostream>
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

    uint16_t operator [](uint32_t index) const;

    inline size_t ElementCount() const;

private:
    std::unique_ptr<uint16_t[]> mBuffer;
    size_t mElementCount;
};

inline size_t IndexBuffer::ElementCount() const
{
    return mElementCount;
}

}
