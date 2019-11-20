#include "renderer.h"
#include "camera.h"
#include "mesh.h"
#include "staticmesh.h"
#include "texture2d.h"
#include "wiidisplaylist.h"
#include "freetypegx.h"
#include "wiidefines.h"
#include "wiisprite.h"
#include "renderdata.h"

renderer::Renderer::Renderer(bool useVSync)
{
    mRenderData = new renderer::RenderData();
    mRenderData->mUseVSync = false;
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

    VIDEO_Configure(mRenderData->mRmode);
    VIDEO_SetNextFramebuffer(mRenderData->mFrameBuffers[mRenderData->mFrameBufferIndex]);
    VIDEO_Flush();
    VIDEO_WaitVSync();
    if (mRenderData->mRmode->viTVMode & VI_NON_INTERLACE)
    {
        VIDEO_WaitVSync();
    }

    mRenderData->mFifoBuffer = memalign(32, renderer::RenderData::DEFAULT_FIFO_SIZE);
    memset(mRenderData->mFifoBuffer, 0, renderer::RenderData::DEFAULT_FIFO_SIZE);
    GX_Init(mRenderData->mFifoBuffer, renderer::RenderData::DEFAULT_FIFO_SIZE);

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


    GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GX_SetAlphaUpdate(GX_TRUE);

    // TODO does not really belong here
    GX_SetNumChans(1);
    GX_SetNumTexGens(1);
    GX_SetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);

    GX_SetViewport(0.0f, 0.0f, mRenderData->mRmode->fbWidth, mRenderData->mRmode->efbHeight, 0.0f, 1.0f);
    GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GX_SetAlphaUpdate(GX_TRUE);
    GX_SetAlphaCompare(GX_GREATER, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GX_SetColorUpdate(GX_ENABLE);

    GX_ClearVtxDesc();
    GX_InvalidateTexAll();
    GX_InvVtxCache();

    VIDEO_SetBlack(false);

    mRenderData->mWidth = mRenderData->mRmode->fbWidth;
    mRenderData->mHeight = mRenderData->mRmode->efbHeight;

    mRenderData->mDefaultFontVertexFormat.SetFormatIndex(GX_VTXFMT1);
    mRenderData->mDefaultFontVertexFormat.AddAttribute({GX_DIRECT, GX_VA_POS, GX_POS_XY, GX_S16});
    mRenderData->mDefaultFontVertexFormat.AddAttribute({GX_DIRECT, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8});
    mRenderData->mDefaultFontVertexFormat.AddAttribute({GX_DIRECT, GX_VA_TEX0, GX_TEX_ST, GX_F32});

    mRenderData->mFreeType = new FreeTypeGX(GX_TF_RGBA8, GX_VTXFMT1);

    mRenderData->mDefaultSpriteVertexFormat.SetFormatIndex(GX_VTXFMT0);
    mRenderData->mDefaultSpriteVertexFormat.AddAttribute({GX_DIRECT, GX_VA_POS, GX_POS_XYZ, GX_F32});
    mRenderData->mDefaultSpriteVertexFormat.AddAttribute({GX_DIRECT, GX_VA_TEX0, GX_TEX_ST, GX_F32});
    mRenderData->mDefaultSpriteVertexFormat.AddAttribute({GX_DIRECT, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8});
}

renderer::Renderer::~Renderer()
{
    delete mRenderData->mFreeType;
    free(MEM_K1_TO_K0(mRenderData->mFrameBuffers[0]));
    free(MEM_K1_TO_K0(mRenderData->mFrameBuffers[1]));
    free(mRenderData->mFifoBuffer);
    delete mRenderData;
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

void renderer::Renderer::PreDraw()
{
    GX_InvVtxCache();
}


void renderer::Renderer::DisplayBuffer()
{
    GX_SetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GX_SetColorUpdate(GX_TRUE);
    GX_SetAlphaUpdate(GX_TRUE);

    mRenderData->mFrameBufferIndex ^= 1; // Flip FrameBuffer
    GX_CopyDisp(mRenderData->mFrameBuffers[mRenderData->mFrameBufferIndex], GX_TRUE);

    GX_DrawDone();
    GX_InvalidateTexAll();
    VIDEO_SetNextFramebuffer(mRenderData->mFrameBuffers[mRenderData->mFrameBufferIndex]);
    VIDEO_Flush();    
    if (mRenderData->mUseVSync)
    {
        VIDEO_WaitVSync();
    }
    else if (mRenderData->mRmode->viTVMode &VI_NON_INTERLACE)
    {
        VIDEO_WaitVSync();
        VIDEO_WaitVSync();
    }
}

void renderer::Renderer::SetCamera(renderer::Camera* camera)
{
    assert(camera != nullptr);
    mCamera = camera;
    math::Matrix4x4 mtx = mCamera->GetProjectionMatrix4x4();
    if (mCamera->IsPerspective())
    {        
        GX_LoadProjectionMtx(mtx.mMatrix, GX_PERSPECTIVE);
    }
    else
    {        
        GX_LoadProjectionMtx(mtx.mMatrix, GX_ORTHOGRAPHIC);
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

void renderer::Renderer::LoadModelViewMatrix(const math::Matrix3x4 &modelView, const uint8_t matrixIndex)
{
    GX_LoadPosMtxImm(const_cast<math::Matrix3x4&>(modelView).mMtx34, matrixIndex);
}

void renderer::Renderer::LoadFont(const uint8_t *fontData, const int32_t size, const uint32_t fontSize)
{
    mRenderData->mFreeType->loadFont(fontData, size, fontSize, false);
}

void renderer::Renderer::DrawText(int32_t x, int32_t y, const std::wstring& text, const ColorRGBA& color)
{
    mRenderData->mDefaultFontVertexFormat.Bind();

    mRenderData->mFreeType->drawText(x, y, text.data(), {color.Red(), color.Green(), color.Blue(), color.Alpha()}, FTGX_JUSTIFY_LEFT);
}

void renderer::Renderer::Draw(Mesh &mesh)
{
    mesh.GetVertexArray()->Bind();
    if (mesh.HasTexture())
    {
        mesh.GetTexture()->Bind(0);
    }
    else
    {
        GX_SetNumTexGens(0);
        GX_SetNumTevStages(1);
        GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORDNULL, GX_TEXMAP_NULL, GX_COLOR0A0);
        GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    }

    const std::shared_ptr<const IndexBuffer> indexBuffer = mesh.GetIndexBuffer();
    const VertexBufferMap& vertexBuffers = mesh.GetVertexArray()->GetVertexBufferMap();
    const uint16_t vertices = static_cast<uint16_t>(indexBuffer->GetElementCount() / vertexBuffers.size());
    const uint16_t indexCount = static_cast<uint16_t>(indexBuffer->GetElementCount());

    GX_Begin(mesh.GetPrimitiveType(), mesh.GetVertexFormatIndex(), vertices);
    for (uint16_t i = 0; i < indexCount;)
    {
        for (const auto& vertexAttribute : vertexBuffers)
        {
            const uint16_t index = indexBuffer->GetIndexAt(i++);

            switch (vertexAttribute.first)
            {
                case GX_VA_POS:
                    GX_Position1x16(index);
                    break;
                case GX_VA_CLR0:
                case GX_VA_CLR1:
                    GX_Color1x16(index);
                    break;
                case GX_VA_TEX0:
                case GX_VA_TEX1:
                case GX_VA_TEX2:
                case GX_VA_TEX3:
                case GX_VA_TEX4:
                case GX_VA_TEX5:
                case GX_VA_TEX6:
                case GX_VA_TEX7:
                    GX_TexCoord1x16(index);
                    break;
                case GX_VA_NRM:
                    GX_Normal1x16(index);
                    break;
                default:
                    assert(false);
                    break;
            }
        }

    }
    GX_End();
}

void renderer::Renderer::Draw(renderer::Sprite &sprite)
{
    mRenderData->mDefaultSpriteVertexFormat.Bind();
    sprite.Bind(0);

    const float width = sprite.Width() * .5f;
    const float height = sprite.Height() * .5f;
    const ColorRGBA& color = sprite.GetColor();

    GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
        GX_Position3f32(-width, -height, 0);
        GX_Color4u8(color.Red(), color.Green(), color.Blue(), color.Alpha());
        GX_TexCoord2f32(0, 0);

        GX_Position3f32(width, -height, 0);
        GX_Color4u8(color.Red(), color.Green(), color.Blue(), color.Alpha());
        GX_TexCoord2f32(1, 0);

        GX_Position3f32(width, height, 0);
        GX_Color4u8(color.Red(), color.Green(), color.Blue(), color.Alpha());
        GX_TexCoord2f32(1, 1);

        GX_Position3f32(-width, height, 0);
        GX_Color4u8(color.Red(), color.Green(), color.Blue(), color.Alpha());
        GX_TexCoord2f32(0, 1);
    GX_End();
}

void renderer::Renderer::Draw(renderer::StaticMesh& staticMesh)
{
    if (staticMesh.IsDirty())
    {
        size_t displayListSize = staticMesh.mMesh->GetIndexBuffer()->GetElementCount()
                * staticMesh.mMesh->GetVertexArray()->GetVertexBufferMap().size() * 2;
        displayListSize *= 2;
        staticMesh.mDisplayList->Begin(displayListSize);
        Draw(*staticMesh.mMesh);
        staticMesh.mDisplayList->End();
        staticMesh.mIsDirty = false;
        staticMesh.mDisplayList->Render();
    }
    else
    {
        staticMesh.mMesh->GetVertexArray()->Bind();
        if (staticMesh.mMesh->HasTexture())
        {
            staticMesh.mMesh->GetTexture()->Bind(0);
        }
        staticMesh.mDisplayList->Render();
    }
}

uint32_t renderer::Renderer::GetWidth() const
{
    return mRenderData->mWidth;
}

uint32_t renderer::Renderer::GetHeight() const
{
    return mRenderData->mHeight;
}
