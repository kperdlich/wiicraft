#pragma once

#include "wiidefines.h"
#include "matrix3x4.h"

namespace renderer {

class Image2D;
class Renderer;

class Sprite {
public:
    explicit Sprite(const Image2D& image);
    ~Sprite() = default;
    Sprite(const Sprite&) = delete;
    Sprite(Sprite&&) = delete;
    void operator=(const Sprite&) = delete;
    void operator=(Sprite&&) = delete;

    void Bind(Renderer& renderer, const uint8_t unit);

    inline uint32_t Width() const;
    inline uint32_t Height() const;

    inline float PosX() const;
    inline void SetPosX(const float posX);

    inline float PosY() const;
    inline void SetPosY(const float posY);

    inline float ScaleX() const;
    inline void SetScaleX(const float scaleX);

    inline float ScaleY() const;
    inline void SetScaleY(const float scaleY);

    inline float RotationDeg() const;
    inline void SetRotationDeg(const float rotation);

private:
    GXTexObj mTexObj;
    float mPosX, mPosY;
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

inline float Sprite::PosY() const
{
    return mPosY;
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

inline float Sprite::PosX() const
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

}