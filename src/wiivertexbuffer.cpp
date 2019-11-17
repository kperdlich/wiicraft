#include <iostream>
#include <malloc.h>
#include "vertexbuffer.h"
#include "wiidefines.h"

renderer::VertexBuffer::VertexBuffer(const std::vector<float>& elements, uint8_t stride)
    : mStride(stride)
{
    mBufferSize = elements.size() * sizeof(float);
    mBuffer = (uint8_t*) memalign(32, mBufferSize);    
    std::copy(elements.begin(), elements.end(), (float*) mBuffer);
    DCFlushRange(mBuffer, mBufferSize);
}

renderer::VertexBuffer::~VertexBuffer()
{
    free(mBuffer);
}
