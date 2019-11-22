#pragma once

#include <memory>
#include "wii_defines.h"
#include "vertexarray.h"
#include "vertexformat.h"

namespace renderer {
class RenderData
{
public:
    static constexpr uint32_t DEFAULT_FIFO_SIZE = 256*1024;

    RenderData() = default;
    ~RenderData()= default;
    RenderData(const RenderData&) = delete;
    RenderData& operator = (const RenderData&) = delete;
    RenderData(RenderData&&) = delete;
    RenderData& operator = (RenderData&&) = delete;

    inline void SetVertexArray(VertexArray *vertexFormat);

public:
    bool mUseVSync;
    uint8_t mFrameBufferIndex;
    void* mFrameBuffers[2];
    void* mFifoBuffer; // Buffer the CPU uses to send commands to GPU
    GXRModeObj* mRmode;
    uint32_t mWidth;
    uint32_t mHeight;
    GXColor mClearColor;
    VertexArray* mVertexArray;

    FreeTypeGX* mFreeType;

    VertexFormat mDefaultSpriteVertexFormat;
    VertexFormat mDefaultFontVertexFormat;
    VertexFormat mDefaultLineVertexFormat;

};

inline void RenderData::SetVertexArray(VertexArray* vertexArray)
{
    mVertexArray = vertexArray;
}

}
