#pragma once

#include "wii_defines.h"
#include "colorrgba.h"
#include "matrix3x4.h"

namespace renderer {

class Image2D;
class Renderer;

class Sprite
{
    friend class Renderer;

public:
    explicit Sprite(const Image2D& image);
    ~Sprite() = default;
    Sprite(const Sprite&) = delete;
    Sprite(Sprite&&) = delete;
    void operator=(const Sprite&) = delete;
    void operator=(Sprite&&) = delete;    

    math::Matrix3x4 GetModelMatrix() const;

    inline uint32_t Width() const;
    inline uint32_t Height() const;

    inline float GetX() const;
    inline void SetPosX(const float posX);

    inline float GetY() const;
    inline void SetPosY(const float posY);

    inline float GetZ() const;
    inline void SetPosZ(const float posZ);

    inline float ScaleX() const;
    inline void SetScaleX(const float scaleX);

    inline float ScaleY() const;
    inline void SetScaleY(const float scaleY);

    inline float RotationDeg() const;
    inline void SetRotationDeg(const float rotation);

    inline void Translate(const float x, const float y, const float z);
    inline void Rotate(const float degree);

    inline void SetColor(const renderer::ColorRGBA& color);
    inline const ColorRGBA& GetColor() const;

private:
    void Bind(const uint8_t unit);

    GXTexObj mTexObj;
    renderer::ColorRGBA mColor;
    float mPosX, mPosY, mPosZ;
    float mScaleX, mScaleY;
    float mRotationDeg;
    uint32_t mWidth, mHeight;
};

inline uint32_t Sprite::Height() const
{
    return mHeight;
}

inline uint32_t Sprite::Width() const
{
    return mWidth;
}

inline float Sprite::ScaleY() const
{
    return mScaleY;
}

inline void Sprite::SetScaleY(const float scaleY)
{
    mScaleY = scaleY;
}

inline float Sprite::GetY() const
{
    return mPosY;
}

inline float Sprite::GetZ() const
{
    return mPosZ;
}

inline void Sprite::SetPosZ(const float posZ)
{
    mPosZ = posZ;
}

inline void Sprite::SetPosY(const float posY)
{
    mPosY = posY;
}

inline float Sprite::ScaleX() const
{
    return mScaleX;
}

inline void Sprite::SetScaleX(const float scaleX)
{
    mScaleX = scaleX;
}

inline float Sprite::GetX() const
{
    return mPosX;
}

inline void Sprite::SetPosX(const float posX)
{
    mPosX = posX;
}

inline float Sprite::RotationDeg() const
{
    return mRotationDeg;
}

inline void Sprite::SetRotationDeg(const float degree)
{
    mRotationDeg = degree;
}

inline void renderer::Sprite::Translate(const float x, const float y, const float z)
{
    mPosX += x;
    mPosY += y;
    mPosZ += z;
}

inline void renderer::Sprite::Rotate(const float degree)
{
    mRotationDeg += degree;
}

inline void Sprite::SetColor(const ColorRGBA &color)
{
    mColor = color;
}

inline const ColorRGBA& Sprite::GetColor() const
{
    return mColor;
}

}
