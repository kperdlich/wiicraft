#pragma once

#include "matrix4x4.h"
#include "mathhelper.h"

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

    void ExtractPlanesD3D(const renderer::Camera& camera, bool normalize);
    void NormalizePlane(Plane & plane);
    float DistanceToPoint(const Plane & plane, const math::Vertex3f & point);
    Halfspace ClassifyPoint(const Plane & plane, const math::Vertex3f & point) const;
    bool IsPointVisible(const math::Vertex3f& point) const;

    inline const Plane* GetPlanes() const;

private:
    Plane mPlanes[6];
};

inline const Plane* Frustrum::GetPlanes() const
{
    return mPlanes;
}

}

