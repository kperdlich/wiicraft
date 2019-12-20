#include "image2d.h"
#include "core.h"
#include "tpl_loader.h"
#include "wii_defines.h"

constexpr uint8_t BytesPerPixelRGBA8 = 4;

renderer::Image2D::Image2D(const uint8_t *data, size_t imageSize)
    : mData(nullptr)
{
    ASSERT(data != nullptr);
    // check for png signature
    if (data[0] == 0x89 && data[1] == 0x50 && data[2] == 0x4E && data[3] == 0x47)
    {
        mFormat = ImageFormat::PNG;
        PNGUPROP pngProps;
        IMGCTX context = PNGU_SelectImageFromBuffer(data);
        PNGU_GetImageProperties(context, &pngProps);
        mData = PNGU_DecodeTo4x4RGBA8(context, pngProps.imgWidth, pngProps.imgHeight, &mWidth, &mHeight, nullptr);
        mDataSize = BytesPerPixelRGBA8 * pngProps.imgWidth * pngProps.imgHeight;
        ASSERT(static_cast<int>(pngProps.imgWidth) == mWidth);
        ASSERT(static_cast<int>(pngProps.imgHeight) == mHeight);
        ASSERT(mData != nullptr);
        PNGU_ReleaseImageContext(context);
        DCFlushRange(mData, mDataSize);
    }
    else if (IsTPLTexture(data))
    {
        ASSERT(imageSize > 0);
        TPL_Texture* tplTexture = renderer::GetTPLTexture(data);
        mWidth = tplTexture->width;
        mHeight = tplTexture->height;
        mFormat = ImageFormat::TPL;
        mDataSize = imageSize;
        mData = (uint8_t*) memalign(32, mDataSize);
        memcpy(mData, (void*)data, mDataSize);
        const size_t tplDataSize = GetTPLTextureSize(imageSize);
        DCFlushRange(mData, tplDataSize);
    }
    else
    {
        ASSERT(false);
    }
}

renderer::Image2D::~Image2D()
{
    if (mData)
    {
        free(mData);
        mData = nullptr;
    }
}
