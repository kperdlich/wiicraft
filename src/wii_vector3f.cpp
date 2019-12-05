#include "vector3f.h"
#include <math.h>
#include "wii_defines.h"

const math::Vector3f math::Vector3f::Up = {0.0f, 1.0f, 0.0f};
const math::Vector3f math::Vector3f::Forward = {0.0f, 0.0f, -1.0f};
const math::Vector3f math::Vector3f::Left = {-1.0f, 0.0f, 0.0f};

math::Vector3f::Vector3f()
{
    mVec.mX = 0;
    mVec.mY = 0;
    mVec.mZ = 0;
}

math::Vector3f::Vector3f(float x, float y, float z)
{
    mVec.mX = x;
    mVec.mY = y;
    mVec.mZ = z;
}

math::Vector3f& math::Vector3f::operator +=(const math::Vector3f &other)
{
    guVecAdd((guVector*)(&mVec),
             (guVector*)(&const_cast<Vector3f&>(other).mVec),
             (guVector*)(&mVec));
    return *this;
}

math::Vector3f &math::Vector3f::operator -=(const math::Vector3f &other)
{
    guVecSub((guVector*)(&mVec),
             (guVector*)(&const_cast<Vector3f&>(other).mVec),
             (guVector*)(&mVec));
    return *this;
}

math::Vector3f math::Vector3f::operator -(const math::Vector3f& other) const
{
    Vector3f vec;
    guVecSub((guVector*)(&mVec),
             (guVector*)(&const_cast<Vector3f&>(other).mVec),
             (guVector*)(&vec.mVec));
    return vec;
}

math::Vector3f math::Vector3f::operator +(const Vector3f &other) const
{
    Vector3f vec;
    guVecAdd((guVector*)(&mVec),
             (guVector*)(&const_cast<Vector3f&>(other).mVec),
             (guVector*)(&vec.mVec));
    return vec;
}

math::Vector3f math::Vector3f::operator *(float scalar) const
{
    Vector3f vec;
    guVecScale((guVector*)&mVec, (guVector*) &vec.mVec, scalar);
    return vec;
}

void math::Vector3f::Normalize()
{
    guVecNormalize(reinterpret_cast<guVector*>(&mVec));
}

float math::Vector3f::Dot(const math::Vector3f& other) const
{
    return guVecDotProduct(reinterpret_cast<guVector*>(&const_cast<Vector3f&>(*this).mVec),
                           reinterpret_cast<guVector*>(&const_cast<Vector3f&>(other).mVec));
}

math::Vector3f math::Vector3f::Cross(const math::Vector3f& other) const
{
    Vector3f cross;
    guVecCross(reinterpret_cast<guVector*>(&const_cast<Vector3f&>(*this).mVec),
                      reinterpret_cast<guVector*>(&const_cast<Vector3f&>(other).mVec),
                      reinterpret_cast<guVector*>(&cross.mVec));
    return cross;
}

float math::Vector3f::Length() const
{
    return sqrt(mVec.mX * mVec.mX + mVec.mY * mVec.mY + mVec.mZ * mVec.mZ);
}


