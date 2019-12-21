#include "indexbuffer.h"


renderer::IndexBuffer::IndexBuffer(const std::initializer_list<uint16_t>& indecies)
{
    mElementCount = indecies.size();
    mBuffer = std::make_unique<uint16_t[]>(mElementCount);
    std::copy(indecies.begin(), indecies.end(), mBuffer.get());
}


