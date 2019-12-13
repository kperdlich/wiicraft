#pragma once

#include <stdint.h>
#include "core.h"
#include "vector3f.h"

namespace math {
class Matrix4x4;

// The elements of the 3x4 matrix are stored in
// row-major order.
class Matrix3x4
{
public:
    explicit Matrix3x4(bool zero = false);
    Matrix3x4(float m34[3][4]);
    Matrix3x4(float m00, float m01, float m02, float m03,
            float m10, float m11, float m12, float m13,
            float m20, float m21, float m22, float m23);
    ~Matrix3x4() = default;
    Matrix3x4(const Matrix3x4&) = default;
    Matrix3x4& operator = (const Matrix3x4&) = default;
    Matrix3x4(Matrix3x4&&) = default;
    Matrix3x4& operator = (Matrix3x4&&) = default;

    Matrix3x4 operator * (const Matrix3x4& other) const;
    Vector3f operator * (const Vector3f& vec) const;
    Matrix4x4 operator * (const Matrix4x4& other) const;

    void Scale(float x, float y, float z);
    void Translate(float x, float y, float z);
    void Rotate(const char axis, float degree);

    math::Matrix3x4 Inverse() const;

    void SetIdentity();
    void SetZero();

    inline math::Vector3f GetColum(uint8_t index) const;
    inline const float* operator[] (uint8_t index) const;

    static Matrix3x4 Identity();

public:
    float mMtx34[3][4];
};

inline const float *Matrix3x4::operator[](uint8_t index) const
{
    ASSERT(index >= 0 && index < 3);
    return mMtx34[index];
}

inline math::Vector3f Matrix3x4::GetColum(uint8_t index) const
{
    ASSERT(index >= 0 && index <= 3);
    return {mMtx34[0][index], mMtx34[1][index], mMtx34[2][index]};
}

}
