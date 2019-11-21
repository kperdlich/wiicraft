#pragma once

#include "matrix4x4.h"
#include "vector3f.h"
#include "geometry_data.h"

namespace renderer {

class Camera;

struct Plane
{
    float a, b, c, d;
};

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
    void NormalizePlane(Plane & plane) const;
    float DistanceToPoint(const Plane & plane, const math::Vector3f & point) const;
    Halfspace ClassifyPoint(const Plane & plane, const math::Vector3f &point) const;
    bool IsPointVisible(const math::Vector3f& point) const;
    bool IsBoxVisible(const core::Box& box) const;

    inline const Plane* GetPlanes() const;

private:
    Plane mPlanes[6];
};

inline const Plane* Frustrum::GetPlanes() const
{
    return mPlanes;
}

}

