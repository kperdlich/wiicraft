#include "VertexBuffer.h"
#include "WiiDefines.h"
#include <iostream>
#include <malloc.h>

renderer::VertexBuffer::VertexBuffer(const void* data, uint8_t size, uint8_t stride)
    : mStride(stride)
{
    mBufferSize = size;
    mBuffer = (uint8_t*)memalign(32, size);
    memcpy(mBuffer, data, mBufferSize);
    DCFlushRange(mBuffer, mBufferSize);
}

renderer::VertexBuffer::~VertexBuffer()
{
    free(mBuffer);
}
