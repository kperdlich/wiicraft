#pragma once

namespace math {

class Vector4f {
public:
    Vector4f();
    Vector4f(float x, float y, float z, float w);
    ~Vector4f() = default;
    Vector4f(const Vector4f&) = default;
    Vector4f(Vector4f&&) = default;
    Vector4f& operator=(const Vector4f&) = default;
    Vector4f& operator=(Vector4f&&) = default;

    inline float X() const;
    inline float Y() const;
    inline float Z() const;
    inline float W() const;

    inline void SetX(float value);
    inline void SetY(float value);
    inline void SetZ(float value);
    inline void SetW(float value);

    float Dot(const Vector4f &other) const;
    float Length() const;

private:
    struct {
        float mX, mY, mZ, mW;
    } mVec;
};

inline float Vector4f::X() const
{
    return mVec.mX;
}

inline float Vector4f::Y() const
{
    return mVec.mY;
}

inline float Vector4f::Z() const
{
    return mVec.mZ;
}

inline float Vector4f::W() const
{
    return mVec.mW;
}

inline void Vector4f::SetX(float value)
{
    mVec.mX = value;
}

inline void Vector4f::SetY(float value)
{
    mVec.mY = value;
}

inline void Vector4f::SetZ(float value)
{
    mVec.mZ = value;
}

inline void Vector4f::SetW(float value)
{
    mVec.mW = value;
}

}
