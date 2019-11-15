#include "vertexbuffer.h"
#include "wiidefines.h"

renderer::VertexBuffer::VertexBuffer(size_t size)
    : mBufferSize(size)
{
    mBuffer = (char*) memalign(32, mBufferSize);

}

renderer::VertexBuffer::~VertexBuffer()
{
    free(mBuffer);
}
