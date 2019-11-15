#pragma once

#include <stddef.h>

namespace renderer {

class VertexBuffer
{
public:
    explicit VertexBuffer(size_t size);
    ~VertexBuffer();
    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer(VertexBuffer&&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;
    VertexBuffer& operator=(VertexBuffer&&) = delete;




private:
    char* mBuffer;
    size_t mBufferSize;
};

}
