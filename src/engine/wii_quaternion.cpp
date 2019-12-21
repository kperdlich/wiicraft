#include "quaternion.h"
#include "wii_defines.h"

renderer::Quaternion::Quaternion(float x, float y, float z, float w)
{
    mQuaternion.x = x;
    mQuaternion.y = y;
    mQuaternion.z = z;
    mQuaternion.w = w;
}

math::Matrix3x4 renderer::Quaternion::GetRotationMatrix3x4() const
{
    math::Matrix3x4 mtx;
    c_guMtxQuat(mtx.mMtx34, (guQuaternion*) &mQuaternion);
    return mtx;
}
