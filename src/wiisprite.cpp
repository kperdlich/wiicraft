#include "image2d.h"
#include "wiisprite.h"
#include "renderer.h"


renderer::Sprite::Sprite(const Image2D& image)
    : mPosX(0), mPosY(0), mPosZ(0), mScaleX(1.0f), mScaleY(1.0f), mRotationDeg(0.0f)
{
    mWidth = (uint32_t) image.Width();
    mHeight = (uint32_t) image.Height();       

    GX_InitTexObj(&mTexObj, (void*) image.Data(), (uint16_t) mWidth, (uint16_t) mHeight, GX_TF_RGBA8, GX_REPEAT, GX_REPEAT, GX_FALSE);
    GX_InitTexObjLOD(&mTexObj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_1);
}

void renderer::Sprite::Bind(renderer::Renderer& renderer, const uint8_t unit)
{
    GX_SetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GX_SetTexCoordGen(GX_TEXCOORD0 + unit, GX_TG_MTX2x4, GX_TG_TEX0 + unit, GX_IDENTITY);
    GX_SetTevOrder(GX_TEVSTAGE0 + unit, GX_TEXCOORD0 + unit, GX_TEXMAP0 + unit, GX_COLOR0A0);
    GX_LoadTexObj(&mTexObj, GX_TEXMAP0 + unit);
}
