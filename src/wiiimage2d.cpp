#include "image2d.h"
#include "tpl_loader.h"
#include "wiidefines.h"

constexpr uint8_t BytesPerPixelRGBA8 = 4;

renderer::Image2D::Image2D(const uint8_t *data, size_t imageSize)
    : mData(nullptr)
{
    assert(data != nullptr);
    // check for png signature
    if (data[0] == 0x89 && data[1] == 0x50 && data[2] == 0x4E && data[3] == 0x47)
    {
        mFormat = ImageFormat::PNG;
        PNGUPROP pngProps;
        IMGCTX context = PNGU_SelectImageFromBuffer(data);
        PNGU_GetImageProperties(context, &pngProps);
        mData = PNGU_DecodeTo4x4RGBA8(context, pngProps.imgWidth, pngProps.imgHeight, &mWidth, &mHeight, nullptr);
        mDataSize = BytesPerPixelRGBA8 * pngProps.imgWidth * pngProps.imgHeight;
        assert(static_cast<int>(pngProps.imgWidth) == mWidth);
        assert(static_cast<int>(pngProps.imgHeight) == mHeight);
        assert(mData != nullptr);
        PNGU_ReleaseImageContext(context);
        DCFlushRange(mData, mDataSize);
    }
    else if (IsTPLTexture(data))
    {
        assert(imageSize > 0);
        mFormat = ImageFormat::TPL;
        mDataSize = imageSize;
        mData = (uint8_t*) memalign(32, mDataSize);
        memcpy(mData, (void*)data, mDataSize);
        const size_t tplDataSize = GetTPLTextureSize(imageSize);
        DCFlushRange(mData, tplDataSize);
    }
    else
    {
        assert(false);
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
