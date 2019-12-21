#pragma once

#include "matrix3x4.h"

namespace renderer {
class Quaternion
{
public:
    Quaternion(float x, float y, float z, float w);
    ~Quaternion() = default;
    Quaternion(const Quaternion&) = default;
    Quaternion& operator = (const Quaternion&) = default;
    Quaternion(Quaternion&&) = default;
    Quaternion& operator = (Quaternion&&) = default;

    math::Matrix3x4 GetRotationMatrix3x4() const;

private:
    struct
    {
        float x, y, z, w;
    } mQuaternion;
};
}
