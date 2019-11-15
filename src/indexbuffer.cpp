#include "indexbuffer.h"
#include <assert.h>


renderer::IndexBuffer::IndexBuffer(const std::initializer_list<uint16_t>& indecies)
{
    mElementCount = indecies.size();
    mBuffer = std::make_unique<uint16_t[]>(mElementCount);
    std::copy(indecies.begin(), indecies.end(), mBuffer.get());
}

uint16_t renderer::IndexBuffer::operator [](uint32_t index) const
{
    assert(index >= 0 && index < mElementCount);
    return mBuffer[index];
}
