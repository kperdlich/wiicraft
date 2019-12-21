#include "renderer.h"
#include "camera.h"
#include "mesh.h"
#include "staticmesh.h"
#include "texture2d.h"
#include "wii_displaylist.h"
#include "freetypegx.h"
#include "wii_defines.h"
#include "wii_sprite.h"
#include "renderdata.h"

renderer::Renderer* renderer::Renderer::s_Renderer = nullptr;


renderer::Renderer::Renderer(bool useVSync)
{
    s_Renderer = this;
    mRenderData = new renderer::RenderData();
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

    mRenderData->mFreeType = new FreeTypeGX(GX_TF_RGBA8, GX_VTXFMT1);

    mRenderData->mDefaultFontVertexFormat.SetFormatIndex(GX_VTXFMT1);
    mRenderData->mDefaultFontVertexFormat.AddAttribute({GX_DIRECT, GX_VA_POS, GX_POS_XY, GX_S16});
    mRenderData->mDefaultFontVertexFormat.AddAttribute({GX_DIRECT, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8});
    mRenderData->mDefaultFontVertexFormat.AddAttribute({GX_DIRECT, GX_VA_TEX0, GX_TEX_ST, GX_F32});

    mRenderData->mDefaultSpriteVertexFormat.SetFormatIndex(GX_VTXFMT0);
    mRenderData->mDefaultSpriteVertexFormat.AddAttribute({GX_DIRECT, GX_VA_POS, GX_POS_XYZ, GX_F32});
    mRenderData->mDefaultSpriteVertexFormat.AddAttribute({GX_DIRECT, GX_VA_TEX0, GX_TEX_ST, GX_F32});
    mRenderData->mDefaultSpriteVertexFormat.AddAttribute({GX_DIRECT, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8});

    mRenderData->mDefaultLineVertexFormat.SetFormatIndex(GX_VTXFMT2);
    mRenderData->mDefaultLineVertexFormat.AddAttribute({GX_DIRECT, GX_VA_POS, GX_POS_XYZ, GX_F32});
    mRenderData->mDefaultLineVertexFormat.AddAttribute({GX_DIRECT, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8});
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

void renderer::Renderer::SetCamera(std::shared_ptr<renderer::Camera> camera)
{
    ASSERT(camera != nullptr);
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

void renderer::Renderer::EnableFog(const float startZ, const float endZ, const renderer::ColorRGBA& color)
{
    GX_SetFog(GX_FOG_LIN, startZ, endZ, mCamera->GetFrustrumNear(), mCamera->GetFrustrumFar(), { color.Red(), color.Green(), color.Blue(), 0 });
}

void renderer::Renderer::DisableFog()
{
    GX_SetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, { 0, 0, 0, 0 });
}

void renderer::Renderer::LoadModelViewMatrix(const math::Matrix3x4 &modelView, const uint8_t matrixIndex)
{
    GX_LoadPosMtxImm(const_cast<math::Matrix3x4&>(modelView).mMtx34, matrixIndex);
}

void renderer::Renderer::LoadFont(const uint8_t *fontData, const int32_t size, const uint32_t fontSize)
{
    mRenderData->mFreeType->loadFont(fontData, size, fontSize, false);
}

void renderer::Renderer::SetLineWidth(uint8_t width)
{
    GX_SetLineWidth(width, mRenderData->mDefaultLineVertexFormat.GetFormatIndex());
}

void renderer::Renderer::DrawText(int32_t x, int32_t y, const std::wstring& text, const ColorRGBA& color)
{
    GX_SetNumTexGens(1);
    GX_SetNumTevStages(1);
    GX_SetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    mRenderData->mDefaultFontVertexFormat.Bind();   
    LoadModelViewMatrix(mCamera->GetViewMatrix3x4());
    mRenderData->mFreeType->drawText(x, y, text.data(), {color.Red(), color.Green(), color.Blue(), color.Alpha()}, FTGX_JUSTIFY_LEFT);
}

void renderer::Renderer::DrawSpriteSheet(int32_t x, int32_t y, renderer::Sprite &sprite, uint32_t index, uint32_t tileWidth, uint32_t tileHeight, uint32_t finalSpriteWidth, uint32_t finalSpriteHeight)
{
    const uint32_t rows = sprite.Height() / tileHeight;
    const uint32_t columns = sprite.Width() / tileWidth;
    const uint32_t tileX = (index % columns) * tileWidth;
    const uint32_t tileY = (index / rows) * tileHeight;
    DrawSpriteSheet(x, y, sprite, tileX, tileY, tileWidth, tileHeight, finalSpriteWidth, finalSpriteHeight);
}

void renderer::Renderer::DrawSpriteSheet(int32_t x, int32_t y, renderer::Sprite &sprite, uint32_t tileX, uint32_t tileY, uint32_t tileWidth, uint32_t tileHeight, uint32_t finalSpriteWidth, uint32_t finalSpriteHeight)
{
    mRenderData->mDefaultSpriteVertexFormat.Bind();
    sprite.Bind(0);

    const float width = finalSpriteWidth * .5f;
    const float height = finalSpriteHeight * .5f;

    const ColorRGBA& color = sprite.GetColor();

    const float sTopLeft = static_cast<float>(tileX) / sprite.Width();
    const float tTopLeft = static_cast<float>(tileY) / sprite.Height();

    const float sTopRight = static_cast<float>(tileX + tileWidth) / sprite.Width();
    const float tTopRight = tTopLeft;

    const float sBottomRight = sTopRight;
    const float tBottomRight = static_cast<float>(tileY + tileHeight) / sprite.Height();

    const float sBottomLeft = sTopLeft;
    const float tBottomLeft = tBottomRight;

    math::Matrix3x4 translation;
    translation.SetIdentity();
    translation.Translate(x, y, 0.0f);
    LoadModelViewMatrix(mCamera->GetViewMatrix3x4() * translation);
    GX_Begin(GX_QUADS, mRenderData->mDefaultSpriteVertexFormat.mFormatIndex, 4);
        GX_Position3f32(-width, -height, 0);
        GX_Color1u32(color.Color());
        GX_TexCoord2f32(sTopLeft, tTopLeft);

        GX_Position3f32(width, -height, 0);
        GX_Color1u32(color.Color());
        GX_TexCoord2f32(sTopRight, tTopRight);

        GX_Position3f32(width, height, 0);
        GX_Color1u32(color.Color());
        GX_TexCoord2f32(sBottomRight, tBottomRight);

        GX_Position3f32(-width, height, 0);
        GX_Color1u32(color.Color());
        GX_TexCoord2f32(sBottomLeft, tBottomLeft);
    GX_End();
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
                    ASSERT(false);
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

    LoadModelViewMatrix(mCamera->GetViewMatrix3x4() * sprite.GetModelMatrix());
    GX_Begin(GX_QUADS, mRenderData->mDefaultSpriteVertexFormat.mFormatIndex, 4);
        GX_Position3f32(-width, -height, 0);
        GX_Color1u32(color.Color());
        GX_TexCoord2f32(0, 0);

        GX_Position3f32(width, -height, 0);
        GX_Color1u32(color.Color());
        GX_TexCoord2f32(1, 0);

        GX_Position3f32(width, height, 0);
        GX_Color1u32(color.Color());
        GX_TexCoord2f32(1, 1);

        GX_Position3f32(-width, height, 0);
        GX_Color1u32(color.Color());
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
        staticMesh.mDisplayList->Render();
    }
}

void renderer::Renderer::DrawLine(const math::Vector3f& from, const math::Vector3f& end, const renderer::ColorRGBA &color)
{
    mRenderData->mDefaultLineVertexFormat.Bind();
    GX_SetNumTexGens(0);
    GX_SetNumTevStages(1);
    GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORDNULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    LoadModelViewMatrix(mCamera->GetViewMatrix3x4());
    GX_Begin(GX_LINES, mRenderData->mDefaultLineVertexFormat.GetFormatIndex(), 2);
        GX_Position3f32(from.X(), from.Y(), from.Z());
        GX_Color4u8(color.Red(), color.Green(), color.Blue(), color.Alpha());

        GX_Position3f32(end.X(), end.Y(), end.Z());
        GX_Color4u8(color.Red(), color.Green(), color.Blue(), color.Alpha());
    GX_End();
}

void renderer::Renderer::DrawRay(const math::Vector3f &from, const math::Vector3f& direction, const renderer::ColorRGBA &color)
{
    mRenderData->mDefaultLineVertexFormat.Bind();
    GX_SetNumTexGens(0);
    GX_SetNumTevStages(1);
    GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORDNULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);

    const math::Vector3f& end = from + direction;
    LoadModelViewMatrix(mCamera->GetViewMatrix3x4());
    GX_Begin(GX_LINES, mRenderData->mDefaultLineVertexFormat.GetFormatIndex(), 2);
        GX_Position3f32(from.X(), from.Y(), from.Z());
        GX_Color4u8(color.Red(), color.Green(), color.Blue(), color.Alpha());

        GX_Position3f32(end.X(), end.Y(), end.Z());
        GX_Color4u8(color.Red(), color.Green(), color.Blue(), color.Alpha());
    GX_End();
}

void renderer::Renderer::DrawAABB(const core::AABB &aabb, const renderer::ColorRGBA &color, float scale)
{
    mRenderData->mDefaultLineVertexFormat.Bind();
    GX_SetNumTexGens(0);
    GX_SetNumTevStages(1);
    GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORDNULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);

    math::Vector3f blockPosition = aabb.GetCenter();
    const math::Vector3f& blockHalfSize = aabb.GetHalfWidth() * scale;
    math::Vector3f vertices[8] = {
            { (float)blockPosition.X() - blockHalfSize.X(), (float)blockPosition.Y() + blockHalfSize.Y(), (float)blockPosition.Z() + blockHalfSize.Z() },// v1
            { (float)blockPosition.X() - blockHalfSize.X(), (float)blockPosition.Y() - blockHalfSize.Y(), (float)blockPosition.Z() + blockHalfSize.Z() }, //v2
            { (float)blockPosition.X() + blockHalfSize.X(), (float)blockPosition.Y() - blockHalfSize.Y(), (float)blockPosition.Z() + blockHalfSize.Z() }, //v3
            { (float)blockPosition.X() + blockHalfSize.X(), (float)blockPosition.Y() + blockHalfSize.Y(), (float)blockPosition.Z() + blockHalfSize.Z() }, // v4
            { (float)blockPosition.X() - blockHalfSize.X(), (float)blockPosition.Y() + blockHalfSize.Y(), (float)blockPosition.Z() - blockHalfSize.Z() }, //v5
            { (float)blockPosition.X() + blockHalfSize.X(), (float)blockPosition.Y() + blockHalfSize.Y(), (float)blockPosition.Z() - blockHalfSize.Z() }, // v6
            { (float)blockPosition.X() + blockHalfSize.X(), (float)blockPosition.Y() - blockHalfSize.Y(), (float)blockPosition.Z() - blockHalfSize.Z() }, // v7
            { (float)blockPosition.X() - blockHalfSize.X(), (float)blockPosition.Y() - blockHalfSize.Y(), (float)blockPosition.Z() - blockHalfSize.Z() } // v8
        };

    LoadModelViewMatrix(mCamera->GetViewMatrix3x4());
    GX_Begin(GX_LINESTRIP, GX_VTXFMT0, 16);
            GX_Position3f32(vertices[1].X(), vertices[1].Y(), vertices[1].Z());
            GX_Color1u32(color.Color());
            GX_Position3f32(vertices[0].X(), vertices[0].Y(), vertices[0].Z());
            GX_Color1u32(color.Color());
            GX_Position3f32(vertices[3].X(), vertices[3].Y(), vertices[3].Z());
            GX_Color1u32(color.Color());
            GX_Position3f32(vertices[2].X(), vertices[2].Y(), vertices[2].Z());
            GX_Color1u32(color.Color());
            GX_Position3f32(vertices[1].X(), vertices[1].Y(), vertices[1].Z());
            GX_Color1u32(color.Color());
            GX_Position3f32(vertices[7].X(), vertices[7].Y(), vertices[7].Z());
            GX_Color1u32(color.Color());
            GX_Position3f32(vertices[6].X(), vertices[6].Y(), vertices[6].Z());
            GX_Color1u32(color.Color());
            GX_Position3f32(vertices[2].X(), vertices[2].Y(), vertices[2].Z());
            GX_Color1u32(color.Color());
            GX_Position3f32(vertices[3].X(), vertices[3].Y(), vertices[3].Z());
            GX_Color1u32(color.Color());
            GX_Position3f32(vertices[5].X(), vertices[5].Y(), vertices[5].Z());
            GX_Color1u32(color.Color());
            GX_Position3f32(vertices[6].X(), vertices[6].Y(), vertices[6].Z());
            GX_Color1u32(color.Color());
            GX_Position3f32(vertices[7].X(), vertices[7].Y(), vertices[7].Z());
            GX_Color1u32(color.Color());
            GX_Position3f32(vertices[4].X(), vertices[4].Y(), vertices[4].Z());
            GX_Color1u32(color.Color());
            GX_Position3f32(vertices[5].X(), vertices[5].Y(), vertices[5].Z());
            GX_Color1u32(color.Color());
            GX_Position3f32(vertices[4].X(), vertices[4].Y(), vertices[4].Z());
            GX_Color1u32(color.Color());
            GX_Position3f32(vertices[0].X(), vertices[0].Y(), vertices[0].Z());
            GX_Color1u32(color.Color());
        GX_End();
}

void renderer::Renderer::ClearStatistics()
{
    mStatistics.ChunksInFrustrum = 0;
    mStatistics.CulledChunks = 0;
}

void renderer::Renderer::UpdateFPS()
{
    static uint8_t frameCount = 0;
    static uint32_t fpsLastTime = 0;

    uint32_t currentTime = ticks_to_millisecs(gettime());
    ++frameCount;
    if (currentTime - fpsLastTime > 1000)
    {
        fpsLastTime = currentTime;
        mStatistics.FPS = frameCount;
        frameCount = 0;
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

