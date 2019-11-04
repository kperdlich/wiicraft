#include "vector3f.h"
#include "wiidefines.h"

math::Vector3f::Vector3f(float x, float y, float z)
{
    mVec.mX = x;
    mVec.mY = y;
    mVec.mZ = z;
}

math::Vector3f math::Vector3f::operator -(const math::Vector3f other)
{
    Vector3f vec;
    guVecSub(reinterpret_cast<guVector*>(&mVec),
             reinterpret_cast<guVector*>(&const_cast<Vector3f&>(other).mVec),
             reinterpret_cast<guVector*>(&vec.mVec));
    return vec;
}

math::Vector3f math::Vector3f::operator +(const math::Vector3f other)
{
    Vector3f vec;
    guVecAdd(reinterpret_cast<guVector*>(&mVec),
             reinterpret_cast<guVector*>(&const_cast<Vector3f&>(other).mVec),
             reinterpret_cast<guVector*>(&vec.mVec));
    return vec;
}

void math::Vector3f::Normalize()
{
    guVecNormalize(reinterpret_cast<guVector*>(&mVec));
}

float math::Vector3f::Dot(const math::Vector3f other) const
{
    return guVecDotProduct(reinterpret_cast<guVector*>(&const_cast<Vector3f&>(*this).mVec),
                           reinterpret_cast<guVector*>(&const_cast<Vector3f&>(other).mVec));
}

math::Vector3f math::Vector3f::Cross(const math::Vector3f other) const
{
    Vector3f cross;
    guVecCross(reinterpret_cast<guVector*>(&const_cast<Vector3f&>(*this).mVec),
                      reinterpret_cast<guVector*>(&const_cast<Vector3f&>(other).mVec),
                      reinterpret_cast<guVector*>(&cross.mVec));
    return cross;
}


