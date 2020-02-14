#include "aabb.h"

core::AABB::AABB(const math::Vector3f& center, const math::Vector3f& halfWidth)
    : mCenter(center),
      mHalfWidth(halfWidth)
{
    mMax = {center.X() + halfWidth.X(), center.Y() + halfWidth.Y(), center.Z() + halfWidth.Z()};
    mMin = {center.X() - halfWidth.X(), center.Y() - halfWidth.Y(), center.Z() - halfWidth.Z()};
}

bool core::AABB::CoolidesWith(const core::AABB &box) const
{
    return (mMax.X() > box.mMin.X() &&
            mMin.X() < box.mMax.X() &&
            mMax.Y() > box.mMin.Y() &&
            mMin.Y() < box.mMax.Y() &&
            mMax.Z() > box.mMin.Z() &&
            mMin.Z() < box.mMax.Z());
}

bool core::AABB::CollidesWith(const std::vector<core::AABB> &boxes)
{
    for (const core::AABB& aabb : boxes)
    {
        if (CoolidesWith(aabb))
        {
            return true;
        }
    }
    return false;
}
