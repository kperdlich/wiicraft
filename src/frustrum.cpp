#include "frustrum.h"
#include "camera.h"
#include "math.h"

void renderer::Frustrum::NormalizePlane(renderer::Plane & plane)
{
    float mag = sqrt(plane.a * plane.a + plane.b * plane.b + plane.c * plane.c);
    plane.a = plane.a / mag;
    plane.b = plane.b / mag;
    plane.c = plane.c / mag;
    plane.d = plane.d / mag;
    }

float renderer::Frustrum::DistanceToPoint(const renderer::Plane & plane, const math::Vertex3f & point)
{
    return plane.a*point.x + plane.b*point.y + plane.c*point.z + plane.d;
}

renderer::Halfspace renderer::Frustrum::ClassifyPoint(const renderer::Plane & plane, const math::Vertex3f & point) const
{
    float d = plane.a*point.x + plane.b*point.y + plane.c*point.z + plane.d;
    if (d < 0) return Halfspace::NEGATIVE;
    if (d > 0) return Halfspace::POSITIVE;
    return Halfspace::ON_PLANE;
}

bool renderer::Frustrum::IsPointVisible(const math::Vertex3f &point) const
{
    for (uint8_t i = 0; i < 6; ++i)
    {
        const Halfspace halfspace = ClassifyPoint(mPlanes[i], point);
        if (halfspace == Halfspace::NEGATIVE)
        {
            return false;
        }
    }
    return true;
}

void renderer::Frustrum::ExtractPlanesD3D(const renderer::Camera &camera, bool normalize)
{
    const math::Matrix4x4& comboMatrix = camera.GetProjectionMatrix4x4() * camera.GetViewMatrix3x4();

    // Left clipping plane
    mPlanes[0].a = comboMatrix._41() + comboMatrix._11();
    mPlanes[0].b = comboMatrix._42() + comboMatrix._12();
    mPlanes[0].c = comboMatrix._43() + comboMatrix._13();
    mPlanes[0].d = comboMatrix._44() + comboMatrix._14();
    // Right clipping plane
    mPlanes[1].a = comboMatrix._41() - comboMatrix._11();
    mPlanes[1].b = comboMatrix._42() - comboMatrix._12();
    mPlanes[1].c = comboMatrix._43() - comboMatrix._13();
    mPlanes[1].d = comboMatrix._44() - comboMatrix._14();
    // Top clipping plane
    mPlanes[2].a = comboMatrix._41() - comboMatrix._21();
    mPlanes[2].b = comboMatrix._42() - comboMatrix._22();
    mPlanes[2].c = comboMatrix._43() - comboMatrix._23();
    mPlanes[2].d = comboMatrix._44() - comboMatrix._24();
    // Bottom clipping plane
    mPlanes[3].a = comboMatrix._41() + comboMatrix._21();
    mPlanes[3].b = comboMatrix._42() + comboMatrix._22();
    mPlanes[3].c = comboMatrix._43() + comboMatrix._23();
    mPlanes[3].d = comboMatrix._44() + comboMatrix._24();
    // Near clipping plane
    mPlanes[4].a = comboMatrix._41() + comboMatrix._31();
    mPlanes[4].b = comboMatrix._42() + comboMatrix._32();
    mPlanes[4].c = comboMatrix._43() + comboMatrix._33();
    mPlanes[4].d = comboMatrix._44() + comboMatrix._34();
    // Far clipping plane
    mPlanes[5].a = comboMatrix._41() - comboMatrix._31();
    mPlanes[5].b = comboMatrix._42() - comboMatrix._32();
    mPlanes[5].c = comboMatrix._43() - comboMatrix._33();
    mPlanes[5].d = comboMatrix._44() - comboMatrix._34();
    // Normalize the plane equations, if requested
    if (normalize == true)
    {
        NormalizePlane(mPlanes[0]);
        NormalizePlane(mPlanes[1]);
        NormalizePlane(mPlanes[2]);
        NormalizePlane(mPlanes[3]);
        NormalizePlane(mPlanes[4]);
        NormalizePlane(mPlanes[5]);
    }
}
