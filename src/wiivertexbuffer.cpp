#include <iostream>
#include <malloc.h>
#include "vertexbuffer.h"
#include "wiidefines.h"

renderer::VertexBuffer::VertexBuffer(float* elements, uint32_t elementCount, uint8_t stride)
    : mStride(stride)
{
    mBufferSize = elementCount * sizeof(float);
    mBuffer = (uint8_t*) memalign(32, mBufferSize);
    memcpy(mBuffer, elements, mBufferSize);
    DCFlushRange(mBuffer, mBufferSize);
}

renderer::VertexBuffer::~VertexBuffer()
{
    free(mBuffer);
}
