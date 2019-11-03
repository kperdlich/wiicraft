#include <memory>
#include "renderer.h"
#include "wiidefines.h"

renderer::Renderer::Renderer(bool useVSync)
{
    mRenderData = std::make_unique<RenderData>();
    mRenderData->mUseVSync = useVSync;
    mRenderData->mFrameBufferIndex = 0;

    VIDEO_Init();
    VIDEO_SetBlack(true);
    mRenderData->mRmode = VIDEO_GetPreferredMode(nullptr);

    if (CONF_GetAspectRatio() == CONF_ASPECT_16_9)
    {
        mRenderData->mRmode->viWidth = 678;
        mRenderData->mRmode->viXOrigin = (VI_MAX_WIDTH_NTSC - 678)/2;
    }
    else
    {
        // 4:3
        mRenderData->mRmode->viWidth = 672;
        mRenderData->mRmode->viXOrigin = (VI_MAX_WIDTH_NTSC - 672)/2;
    }

    // allocate framebuffers for double buffering
    mRenderData->mFrameBuffers[0] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(mRenderData->mRmode));
    mRenderData->mFrameBuffers[1] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(mRenderData->mRmode));

    mRenderData->mFifoBuffer = memalign(32, RenderData::DEFAULT_FIFO_SIZE);
    memset(mRenderData->mFifoBuffer, 0, RenderData::DEFAULT_FIFO_SIZE);
    GX_Init(mRenderData->mFifoBuffer, RenderData::DEFAULT_FIFO_SIZE);
    SetClearColor(ColorRGBA::BLACK);

    const f32 yScale = GX_GetYScaleFactor(mRenderData->mRmode->efbHeight, mRenderData->mRmode->xfbHeight);
    const u32 xfbHeight = GX_SetDispCopyYScale(yScale);
    GX_SetDispCopySrc(0, 0, mRenderData->mRmode->fbWidth, mRenderData->mRmode->efbHeight);
    GX_SetDispCopyDst(mRenderData->mRmode->fbWidth, xfbHeight);
    GX_SetCopyFilter(mRenderData->mRmode->aa, mRenderData->mRmode->sample_pattern, GX_TRUE, mRenderData->mRmode->vfilter);
    GX_SetDispCopyGamma(GX_GM_1_0);
    GX_SetFieldMode(mRenderData->mRmode->field_rendering,
                    ((mRenderData->mRmode->viHeight == 2 * mRenderData->mRmode->xfbHeight)
                     ? GX_ENABLE : GX_DISABLE));

    if (mRenderData->mRmode->aa)
    {
        GX_SetPixelFmt(GX_PF_RGB565_Z16, GX_ZC_LINEAR);
    }
    else
    {
        GX_SetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);
    }

    GX_CopyDisp(mRenderData->mFrameBuffers[mRenderData->mFrameBufferIndex], GX_TRUE);
    GX_InvalidateTexAll();
    GX_InvVtxCache();

    VIDEO_Configure(mRenderData->mRmode);
    VIDEO_SetNextFramebuffer(mRenderData->mFrameBuffers[mRenderData->mFrameBufferIndex]);
    VIDEO_SetBlack(false);
    VIDEO_Flush();
    VIDEO_WaitVSync();
    if (mRenderData->mRmode->viTVMode & VI_NON_INTERLACE)
    {
        VIDEO_WaitVSync();
    }

    mRenderData->mWidth = mRenderData->mRmode->fbWidth;
    mRenderData->mHeight = mRenderData->mRmode->efbHeight;
}

void renderer::Renderer::SetClearColor(const renderer::ColorRGBA &clearColor)
{
    GXColor& gxClearColor = mRenderData->mClearColor;
    gxClearColor.r = clearColor.Red();
    gxClearColor.g = clearColor.Green();
    gxClearColor.b = clearColor.Blue();
    gxClearColor.a = clearColor.Alpha();
    GX_SetCopyClear(gxClearColor, GX_MAX_Z24);
}

void renderer::Renderer::DisplayBuffer()
{
    GX_SetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GX_SetColorUpdate(GX_TRUE);
    mRenderData->mFrameBufferIndex ^= 1; // Flip FrameBuffer
    GX_CopyDisp(mRenderData->mFrameBuffers[mRenderData->mFrameBufferIndex], GX_TRUE);

    GX_DrawDone();
    VIDEO_SetNextFramebuffer(mRenderData->mFrameBuffers[mRenderData->mFrameBufferIndex]);
    VIDEO_Flush();
    if (mRenderData->mUseVSync)
    {
        VIDEO_WaitVSync();
    }
}
