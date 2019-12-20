#include "texture2d.h"
#include "wii_texture2ddata.h"
#include "image2d.h"
#include "tpl_loader.h"
#include "core.h"

renderer::Texture2D::Texture2D(const Image2D& image)
{
    mTextureData = new Texture2DData();

    switch (image.Format())
    {
        case ImageFormat::PNG:
        {
            GX_InitTexObj(&mTextureData->mTexObj, (void*) image.Data(), (uint16_t) image.Width(), (uint16_t) image.Height(), GX_TF_RGBA8, GX_REPEAT, GX_REPEAT, GX_FALSE);
            GX_InitTexObjLOD(&mTextureData->mTexObj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_4);
            break;
        }

        case ImageFormat::TPL:
        {
            const TPL_Texture* tplTexture = GetTPLTexture(const_cast<unsigned char*>(image.Data()));
            GX_InitTexObj(&mTextureData->mTexObj, (void*) (image.Data() + tplTexture->dataOffs), tplTexture->width, tplTexture->width,
                          tplTexture->format, tplTexture->wrap_s, tplTexture->wrap_t, GX_TRUE);
            if (tplTexture->maxLod)
            {
                GX_InitTexObjLOD(&mTextureData->mTexObj, tplTexture->minFilt, tplTexture->magFilt,
                                 tplTexture->minLod, tplTexture->maxLod, tplTexture->lodBias, GX_DISABLE, tplTexture->edgeLod, GX_ANISO_4);

                GX_InitTexObjEdgeLOD(&mTextureData->mTexObj, GX_ENABLE);
                GX_InitTexObjMaxAniso(&mTextureData->mTexObj, GX_ANISO_4);
            }
            else
            {
                GX_InitTexObjFilterMode(&mTextureData->mTexObj, tplTexture->minFilt, tplTexture->magFilt);
            }
            break;
        }
      default:
        ASSERT(false);
    }

}

renderer::Texture2D::~Texture2D()
{
    delete mTextureData;
}

void renderer::Texture2D::Bind(uint8_t unit)
{
    GX_SetNumTexGens(1);
    GX_SetNumTevStages(1);
    GX_SetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GX_SetTexCoordGen(GX_TEXCOORD0 + unit, GX_TG_MTX2x4, GX_TG_TEX0 + unit, GX_IDENTITY);
    GX_SetTevOrder(GX_TEVSTAGE0 + unit, GX_TEXCOORD0 + unit, GX_TEXMAP0 + unit, GX_COLOR0A0);
    GX_LoadTexObj(&mTextureData->mTexObj, GX_TEXMAP0 + unit);
}
