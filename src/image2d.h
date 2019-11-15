#pragma once

#include <stdint.h>

namespace renderer {

enum class ImageFormat : char
{
    PNG = 0,
    TPL = 1
};

class Image2D
{
public:
    explicit Image2D(const unsigned char *data);
    ~Image2D();
    Image2D(const Image2D&) = delete;
    Image2D(Image2D&&) = delete;
    void operator=(const Image2D&) = delete;
    void operator=(Image2D&&) = delete;

    int Width() const;
    int Height() const;

    ImageFormat Format() const;

    const unsigned char* Data() const;

private:
    int mWidth;
    int mHeight;
    ImageFormat mFormat;
    unsigned char* mData;
};

inline int Image2D::Width() const
{
    return mWidth;
}

inline int Image2D::Height() const
{
    return mHeight;
}

inline ImageFormat Image2D::Format() const
{
    return mFormat;
}

inline const unsigned char* Image2D::Data() const
{
    return mData;
}

}
