#include "image2d.h"
#include <stdlib.h>
#include <assert.h>
#include <pngu.h>

renderer::Image2D::Image2D(const unsigned char *data)
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
        assert(static_cast<int>(pngProps.imgWidth) == mWidth);
        assert(static_cast<int>(pngProps.imgHeight) == mHeight);
        assert(mData != nullptr);
        PNGU_ReleaseImageContext(context);
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
