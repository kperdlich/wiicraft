#pragma once

#include "matrix4x4.h"
#include "vector4f.h"
#include "aabb.h"
#include "geometry_data.h"

namespace renderer {

class Camera;

enum class Halfspace : int8_t
{
     NEGATIVE = -1,
     ON_PLANE = 0,
     POSITIVE = 1,
};

class Frustrum
{
public:
    Frustrum() = default;
    ~Frustrum() = default;
    Frustrum(const Frustrum&) = delete;
    Frustrum& operator = (const Frustrum&) = delete;
    Frustrum(Frustrum&&) = delete;
    Frustrum& operator = (Frustrum&&) = delete;

    void ExtractPlanes(const renderer::Camera& camera, bool normalize);
    void NormalizePlane(math::Vector4f & plane) const;
    float DistanceToPoint(const math::Vector4f & plane, const math::Vector3f & point) const;
    Halfspace ClassifyPoint(const math::Vector4f & plane, const math::Vector3f &point) const;
    bool IsVisible(const math::Vector3f& point) const;
    bool IsVisible(const core::AABB& aabb) const;
    bool IsVisible(const core::Box& box) const;


    inline const math::Vector4f* GetPlanes() const;

private:
    math::Vector4f mPlanes[6];
};

inline const math::Vector4f* Frustrum::GetPlanes() const
{
    return mPlanes;
}

}

