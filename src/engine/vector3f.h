#pragma once

namespace math {
class Vector3f {
    friend class Matrix3x4;
public:
    static const Vector3f Up;
    static const Vector3f Forward;
    static const Vector3f Left;

    Vector3f();
    Vector3f(float x, float y, float z);
    ~Vector3f() = default;
    Vector3f(const Vector3f&) = default;
    Vector3f(Vector3f&&) = default;
    Vector3f& operator=(const Vector3f&) = default;
    Vector3f& operator=(Vector3f&&) = default;

    Vector3f& operator += (const Vector3f& other);
    Vector3f& operator -= (const Vector3f& other);

    Vector3f operator - (const Vector3f& other) const;
    Vector3f operator + (const Vector3f& other) const;
    Vector3f operator * (float scalar) const;

    inline float X() const;
    inline float Y() const;
    inline float Z() const;

    inline void SetX(float value);
    inline void SetY(float value);
    inline void SetZ(float value);

    void Normalize();
    float Dot(const Vector3f &other) const;
    Vector3f Cross(const Vector3f& other) const;
    float Length() const;

private:
    struct {
        float mX, mY, mZ;
    } mVec;
};

inline float Vector3f::X() const
{
    return mVec.mX;
}

inline float Vector3f::Y() const
{
    return mVec.mY;
}

inline float Vector3f::Z() const
{
    return mVec.mZ;
}

inline void Vector3f::SetX(float value)
{
    mVec.mX = value;
}

inline void Vector3f::SetY(float value)
{
    mVec.mY = value;
}

inline void Vector3f::SetZ(float value)
{
    mVec.mZ = value;
}

}
