#include "wii_displaylist.h"
#include "core.h"

renderer::DisplayList::DisplayList()
    : mDispList(nullptr),
      mBufferSize(0) {}

renderer::DisplayList::DisplayList(const size_t bufferSize)
    : mDispList(nullptr),
      mBufferSize(0)
{
    Begin(bufferSize);
}

renderer::DisplayList::~DisplayList()
{
	Clear();
}

void renderer::DisplayList::Render()
{
    if (mDispList)
    {
        GX_CallDispList(mDispList, mBufferSize);
    }    
}

void renderer::DisplayList::Begin(const size_t bufferSize)
{
    Clear();
    ASSERT(bufferSize > 0);
    mDispList = memalign(32, bufferSize);
    memset(mDispList, 0, bufferSize);
    DCInvalidateRange(mDispList, bufferSize);
    GX_BeginDispList(mDispList, bufferSize);
}


void renderer::DisplayList::End()
{
    mBufferSize = GX_EndDispList();
    ASSERT(mBufferSize != 0);
    // Update DisplayList size to the size returned by GX_EndDispList() to save memory
    realloc(mDispList, mBufferSize);
    DCFlushRange(mDispList, mBufferSize);
}

void renderer::DisplayList::Clear()
{
    if (mDispList)
    {
        free(mDispList);
        mBufferSize = 0;
        mDispList = nullptr;
    }
}
