#pragma once

namespace math {
class Vector3f {
public:
    Vector3f() = default;
    Vector3f(float x, float y, float z);
    ~Vector3f() = default;
    Vector3f(const Vector3f&) = default;
    Vector3f(Vector3f&&) = default;
    Vector3f& operator=(const Vector3f&) = default;
    Vector3f& operator=(Vector3f&&) = default;

    Vector3f operator - (const Vector3f other);
    Vector3f operator + (const Vector3f other);

    inline float X() const;
    inline float Y() const;
    inline float Z() const;

    void Normalize();
    float Dot(const Vector3f other) const;
    Vector3f Cross(const Vector3f other) const;
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

}
