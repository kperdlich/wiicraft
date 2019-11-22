#pragma once

#include "vector3f.h"

namespace core {

class AABB
{
public:
    AABB(const math::Vector3f& center, const math::Vector3f& halfWidth);
    ~AABB() = default;
    AABB(const AABB&) = default;
    AABB(AABB&&) = default;
    AABB& operator=(const AABB&) = default;
    AABB& operator=(AABB&&) = default;

    bool CoolidesWith(const AABB& box) const;

    inline const math::Vector3f& GetMax() const;
    inline const math::Vector3f& GetMin() const;

private:
    math::Vector3f mMin;
    math::Vector3f mMax;
};

inline const math::Vector3f &AABB::GetMax() const
{
    return mMax;
}

inline const math::Vector3f &AABB::GetMin() const
{
    return mMin;
}

}
