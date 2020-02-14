#pragma once

#include <vector>
#include "vector3f.h"

namespace core {

class AABB
{
public:
    AABB() = default;
    AABB(const math::Vector3f& center, const math::Vector3f& halfWidth);
    ~AABB() = default;
    AABB(const AABB&) = default;
    AABB(AABB&&) = default;
    AABB& operator=(const AABB&) = default;
    AABB& operator=(AABB&&) = default;

    bool CoolidesWith(const AABB& box) const;
    bool CollidesWith(const std::vector<core::AABB> &boxes);

    inline const math::Vector3f& GetMax() const;
    inline const math::Vector3f& GetMin() const;
    inline const math::Vector3f& GetCenter() const;
    inline const math::Vector3f& GetHalfWidth() const;

private:
    math::Vector3f mCenter;
    math::Vector3f mHalfWidth;
    math::Vector3f mMin;
    math::Vector3f mMax;
};

inline const math::Vector3f& AABB::GetCenter() const
{
    return mCenter;
}

inline const math::Vector3f& AABB::GetHalfWidth() const
{
    return mHalfWidth;
}

inline const math::Vector3f &AABB::GetMax() const
{
    return mMax;
}

inline const math::Vector3f &AABB::GetMin() const
{
    return mMin;
}

}
