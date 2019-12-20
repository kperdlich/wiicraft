#include "image2d.h"
#include "wii_sprite.h"
#include "renderer.h"


renderer::Sprite::Sprite(const Image2D& image)
    : mPosX(0), mPosY(0), mPosZ(0), mScaleX(1.0f), mScaleY(1.0f), mRotationDeg(0.0f)
{
    mWidth = (uint32_t) image.Width();
    mHeight = (uint32_t) image.Height();       

    GX_InitTexObj(&mTexObj, (void*) image.Data(), (uint16_t) mWidth, (uint16_t) mHeight, GX_TF_RGBA8, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GX_InitTexObjLOD(&mTexObj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_4);
}

math::Matrix3x4 renderer::Sprite::GetModelMatrix() const
{
    math::Matrix3x4 translation, scale, rotation;
    translation.SetIdentity();
    scale.SetIdentity();
    rotation.SetIdentity();
    scale.Scale(mScaleX, mScaleY, 0.0f);
    translation.Translate(mPosX, mPosY, mPosZ);
    rotation.Rotate('Z', mRotationDeg);
    return translation * rotation * scale;
}

void renderer::Sprite::Bind(const uint8_t unit)
{
    GX_SetNumTevStages(1);
    GX_SetNumTexGens(1);
    GX_SetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GX_SetTevOrder(GX_TEVSTAGE0 + unit, GX_TEXCOORD0 + unit, GX_TEXMAP0 + unit, GX_COLOR0A0);
    GX_SetTexCoordGen(GX_TEXCOORD0 + unit, GX_TG_MTX2x4, GX_TG_TEX0 + unit, GX_IDENTITY);
    GX_LoadTexObj(&mTexObj, GX_TEXMAP0 + unit);
}
