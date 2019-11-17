#include "wiitexture2d.h"
#include "image2d.h"
#include "tpl_loader.h"
#include <assert.h>

renderer::Texture2D::Texture2D(const Image2D& image)
{
    switch (image.Format())
    {
        case ImageFormat::PNG:
        {
            GX_InitTexObj(&mTexObj, (void*) image.Data(), (uint16_t) image.Width(), (uint16_t) image.Height(), GX_TF_RGBA8, GX_REPEAT, GX_REPEAT, GX_FALSE);
            GX_InitTexObjLOD(&mTexObj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_1);
            break;
        }

        case ImageFormat::TPL:
        {
            const TPL_Texture* tplTexture = GetTPLTexture(const_cast<unsigned char*>(image.Data()));
            GX_InitTexObj(&mTexObj, (void*) (image.Data() + tplTexture->dataOffs), tplTexture->width, tplTexture->width,
                          tplTexture->format, tplTexture->wrap_s, tplTexture->wrap_t, GX_FALSE);
            if (tplTexture->maxLod)
            {
                GX_InitTexObjLOD(&mTexObj, tplTexture->minFilt, tplTexture->magFilt,
                                 tplTexture->minLod, tplTexture->maxLod, tplTexture->lodBias, GX_DISABLE, tplTexture->edgeLod, GX_ANISO_4);

                GX_InitTexObjEdgeLOD(&mTexObj, GX_ENABLE);
                GX_InitTexObjMaxAniso(&mTexObj, GX_ANISO_4);
            }
            else
            {
                GX_InitTexObjFilterMode(&mTexObj, tplTexture->minFilt, tplTexture->magFilt);
            }
            break;
        }
      default:
        assert(false);
    }

}

void renderer::Texture2D::Bind(renderer::Renderer &renderer, uint8_t unit)
{
    GX_SetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GX_SetTexCoordGen(GX_TEXCOORD0 + unit, GX_TG_MTX2x4, GX_TG_TEX0 + unit, GX_IDENTITY);
    GX_SetTevOrder(GX_TEVSTAGE0 + unit, GX_TEXCOORD0 + unit, GX_TEXMAP0 + unit, GX_COLOR0A0);
    GX_LoadTexObj(&mTexObj, GX_TEXMAP0 + unit);
}
