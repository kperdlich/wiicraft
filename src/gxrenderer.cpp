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

void renderer::Renderer::SetCamera(std::shared_ptr<renderer::Camera> camera)
{
    mCamera = camera;
    Mtx mtx;
    if (mCamera->IsPerspective())
    {
        guFrustum(mtx, mCamera->GetFrustrumTop(), mCamera->GetFrustrumBottom(), mCamera->GetFrustrumLeft(), mCamera->GetFrustrumRight(),
                  mCamera->GetFrustrumNear(), mCamera->GetFrustrumFar());
        GX_LoadProjectionMtx(mtx, GX_PERSPECTIVE);
    }
    else
    {
        guOrtho(mtx, mCamera->GetFrustrumTop(), mCamera->GetFrustrumBottom(), mCamera->GetFrustrumLeft(), mCamera->GetFrustrumRight(),
                mCamera->GetFrustrumNear(), mCamera->GetFrustrumFar());
        GX_LoadProjectionMtx(mtx, GX_ORTHOGRAPHIC);
    }
}

void renderer::Renderer::SetZModeEnabled(bool isEnabled)
{
    if (isEnabled)
    {
        GX_SetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    }
    else
    {
        GX_SetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    }
}

void renderer::Renderer::SetCullMode(const CullMode& mode)
{
    switch (mode)
    {
        case CullMode::All:
            GX_SetCullMode(GX_CULL_ALL);
            break;
        case CullMode::None:
            GX_SetCullMode(GX_CULL_NONE);
            break;
        case CullMode::Front:
            GX_SetCullMode(GX_CULL_FRONT);
            break;
        case CullMode::Back:
            GX_SetCullMode(GX_CULL_BACK);
            break;
    }
}

void renderer::Renderer::DummyRender()
{
    GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);

    // GX_ClearVtxDesc();


    GX_SetNumTexGens(0);
    GX_SetNumTevStages(1);
    GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORDNULL, GX_TEXMAP_NULL,
        GX_COLOR0A0);
    GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);

    GX_SetNumChans(1);

    Mtx mtx;
    guMtxIdentity(mtx);
    // guMtxTransApply(mtx, mtx, 0, 0, 0);
    GX_LoadPosMtxImm(mtx, GX_PNMTX0);
    GX_Begin(GX_TRIANGLES, GX_VTXFMT0, 3);
            GX_Position3f32(100, 300, 0);
            GX_Color4u8(255, 255, 255, 255);

            GX_Position3f32(200, 200, 0);
            GX_Color4u8(255, 255, 255, 255);

            GX_Position3f32(300, 300, 0);
            GX_Color4u8(0, 255, 255, 255);
    GX_End();


}

uint32_t renderer::Renderer::GetWidth() const
{
    return mRenderData->mWidth;
}

uint32_t renderer::Renderer::GetHeight() const
{
    return mRenderData->mHeight;
}
