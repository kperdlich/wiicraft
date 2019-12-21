#pragma once

#include <memory>
#include <vector>
#include <stdint.h>
#include <stddef.h>

namespace renderer {

class VertexBuffer
{
public:
    VertexBuffer(const void* data, uint8_t size, uint8_t stride);
    ~VertexBuffer();
    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer(VertexBuffer&&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;
    VertexBuffer& operator=(VertexBuffer&&) = delete;    

    inline uint8_t* GetBuffer();
    inline const uint8_t* GetBuffer() const;
    inline uint8_t GetStride() const;
    inline size_t GetBufferSize() const;

private:
    uint8_t* mBuffer;
    size_t mBufferSize;
    uint8_t mStride;
};

inline uint8_t* VertexBuffer::GetBuffer()
{
    return mBuffer;
}

inline const uint8_t* VertexBuffer::GetBuffer() const
{
    return mBuffer;
}

inline uint8_t VertexBuffer::GetStride() const
{
    return mStride;
}

size_t VertexBuffer::GetBufferSize() const
{
    return mBufferSize;
}

}
