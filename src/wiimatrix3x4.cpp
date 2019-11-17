#include "matrix3x4.h"
#include "wiidefines.h"

math::Matrix3x4::Matrix3x4(bool zero)
{
    if (zero)
    {
        SetZero();
    }
}

math::Matrix3x4::Matrix3x4(float m34[3][4])
{
    guMtxCopy(m34, mMtx34);
}

math::Matrix3x4::Matrix3x4(float m00, float m01, float m02, float m03,
                           float m10, float m11, float m12, float m13,
                           float m20, float m21, float m22, float m23)
{
    mMtx34[0][0] = m00; mMtx34[0][1] = m01;  mMtx34[0][2] = m02;  mMtx34[0][3] = m03;
    mMtx34[1][0] = m10; mMtx34[1][1] = m11;  mMtx34[1][2] = m12;  mMtx34[1][3] = m13;
    mMtx34[2][0] = m20; mMtx34[2][1] = m21;  mMtx34[2][2] = m22;  mMtx34[2][3] = m23;
}


math::Matrix3x4 math::Matrix3x4::operator *(const math::Matrix3x4 &other)
{
    Matrix3x4 mtx;
    guMtxConcat(mMtx34, const_cast<Matrix3x4&>(other).mMtx34, mtx.mMtx34);
    return mtx;
}

void math::Matrix3x4::Scale(float x, float y, float z)
{
    guMtxScaleApply(mMtx34, mMtx34, x, y, z);
}

void math::Matrix3x4::Translate(float x, float y, float z)
{
    guMtxTransApply(mMtx34, mMtx34, x, y, z);
}

void math::Matrix3x4::Rotate(const char axis, float degree)
{
    Mtx mtx;
    guMtxIdentity(mtx);
    guMtxRotDeg(mtx, axis, degree);
    guMtxConcat(mtx, mMtx34, mMtx34);
}

void math::Matrix3x4::SetIdentity()
{
    guMtxIdentity(mMtx34);
}

void math::Matrix3x4::SetZero()
{
    for (uint32_t row = 0; row < 3; ++row)
    {
        for (uint32_t column = 0; column < 3; ++column)
        {
            mMtx34[row][column] = 0;
        }
    }
}

math::Matrix3x4 math::Matrix3x4::Identity()
{
    Matrix3x4 mtx;
    mtx.SetIdentity();
    return mtx;
}

