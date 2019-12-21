#include "matrix3x4.h"
#include "matrix4x4.h"
#include "wii_defines.h"

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


math::Matrix3x4 math::Matrix3x4::operator * (const math::Matrix3x4 &other) const
{
    Mtx mtx;
    guMtxConcat(const_cast<MtxP>(mMtx34), const_cast<Matrix3x4&>(other).mMtx34, mtx);
    return mtx;
}

math::Vector3f math::Matrix3x4::operator *(const math::Vector3f &vec) const
{
    guVector ret;
    guVecMultiply(const_cast<MtxP>(mMtx34), (guVector*) &vec.mVec, &ret);
    return math::Vector3f(ret.x, ret.y, ret.z);
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

math::Matrix3x4 math::Matrix3x4::Inverse() const
{
    Mtx mtx;
    guMtxInverse(const_cast<MtxP>(mMtx34), mtx);
    return mtx;
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

math::Matrix4x4 math::Matrix3x4::operator *(const math::Matrix4x4 &other) const
{
    return math::Matrix4x4(
                mMtx34[0][0] * other[0][0] +
                        mMtx34[0][1] * other[1][0] +
                        mMtx34[0][2] * other[2][0] +
                        mMtx34[0][3] * other[3][0],

                        mMtx34[0][0] * other[0][1] +
                        mMtx34[0][1] * other[1][1] +
                        mMtx34[0][2] * other[2][1] +
                        mMtx34[0][3] * other[3][1],

                        mMtx34[0][0] * other[0][2] +
                        mMtx34[0][1] * other[1][2] +
                        mMtx34[0][2] * other[2][2] +
                        mMtx34[0][3] * other[3][2],

                        mMtx34[0][0] * other[0][3] +
                        mMtx34[0][1] * other[1][3] +
                        mMtx34[0][2] * other[2][3] +
                        mMtx34[0][3] * other[3][3],

                        mMtx34[1][0] * other[0][0] +
                        mMtx34[1][1] * other[1][0] +
                        mMtx34[1][2] * other[2][0] +
                        mMtx34[1][3] * other[3][0],

                        mMtx34[1][0] * other[0][1] +
                        mMtx34[1][1] * other[1][1] +
                        mMtx34[1][2] * other[2][1] +
                        mMtx34[1][3] * other[3][1],

                        mMtx34[1][0] * other[0][2] +
                        mMtx34[1][1] * other[1][2] +
                        mMtx34[1][2] * other[2][2] +
                        mMtx34[1][3] * other[3][2],

                        mMtx34[1][0] * other[0][3] +
                        mMtx34[1][1] * other[1][3] +
                        mMtx34[1][2] * other[2][3] +
                        mMtx34[1][3] * other[3][3],

                        mMtx34[2][0] * other[0][0] +
                        mMtx34[2][1] * other[1][0] +
                        mMtx34[2][2] * other[2][0] +
                        mMtx34[2][3] * other[3][0],

                        mMtx34[2][0] * other[0][1] +
                        mMtx34[2][1] * other[1][1] +
                        mMtx34[2][2] * other[2][1] +
                        mMtx34[2][3] * other[3][1],

                        mMtx34[2][0] * other[0][2] +
                        mMtx34[2][1] * other[1][2] +
                        mMtx34[2][2] * other[2][2] +
                        mMtx34[2][3] * other[3][2],

                        mMtx34[2][0] * other[0][3] +
                        mMtx34[2][1] * other[1][3] +
                        mMtx34[2][2] * other[2][3] +
                        mMtx34[2][3] * other[3][3],

                        other[3][0],
                        other[3][1],
                        other[3][2],
                        other[3][3]);
}
