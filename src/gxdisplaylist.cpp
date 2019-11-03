#include "gxdisplaylist.h"

renderer::DisplayList::DisplayList(const size_t bufferSize)
    : mBufferSize(0)
{
    mDispList = memalign(32, bufferSize);
    memset(mDispList, 0, bufferSize);
    DCInvalidateRange(mDispList, bufferSize);
    GX_BeginDispList(mDispList, bufferSize);
}

renderer::DisplayList::~DisplayList()
{
	Clear();
}

void renderer::DisplayList::Render()
{
    if (mBufferSize > 0)
	{
        GX_CallDispList(mDispList, mBufferSize);
	}
}


void renderer::DisplayList::End()
{
    mBufferSize = GX_EndDispList();
    // Update DisplayList size to the size returned by GX_EndDispList() to save memory
    realloc(mDispList, mBufferSize);
}

void renderer::DisplayList::Clear()
{
    free(mDispList);
    mBufferSize = 0;
    mDispList = nullptr;
}
