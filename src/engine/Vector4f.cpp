#include "Vector4f.h"
#include <math.h>

math::Vector4f::Vector4f()
{
    mVec.mX = 0.0f;
    mVec.mY = 0.0f;
    mVec.mZ = 0.0f;
    mVec.mW = 0.0f;
}

math::Vector4f::Vector4f(float x, float y, float z, float w)
{
    mVec.mX = x;
    mVec.mY = y;
    mVec.mZ = z;
    mVec.mW = w;
}

float math::Vector4f::Dot(const math::Vector4f& other) const
{
    return mVec.mX * other.mVec.mX + mVec.mY * other.mVec.mY + mVec.mZ * other.mVec.mZ + mVec.mW * other.mVec.mW;
}

float math::Vector4f::Length() const
{
    return sqrt(mVec.mX * mVec.mX + mVec.mY * mVec.mY + mVec.mZ * mVec.mZ + mVec.mW * mVec.mW);
}
