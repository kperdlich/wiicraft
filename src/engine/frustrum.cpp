#include <math.h>
#include "frustrum.h"
#include "camera.h"
#include "geometry_data.h"
#include "vector4f.h"

void renderer::Frustrum::NormalizePlane(math::Vector4f & plane) const
{
    float mag = plane.Length();
    plane.SetX(plane.X() / mag);
    plane.SetY(plane.Y() / mag);
    plane.SetZ(plane.Z() / mag);
    plane.SetW(plane.W() / mag);
}

float renderer::Frustrum::DistanceToPoint(const math::Vector4f & plane, const math::Vector3f & point) const
{
    return plane.X() * point.X() + plane.Y() * point.Y() + plane.Z() * point.Z() + plane.W();
}

renderer::Halfspace renderer::Frustrum::ClassifyPoint(const math::Vector4f & plane, const math::Vector3f & point) const
{
    float distance = plane.X()*point.X() + plane.Y()*point.Y() + plane.Z()*point.Z() + plane.W();
    if (distance < 0)
    {
        return Halfspace::NEGATIVE;
    }
    if (distance > 0)
    {
        return Halfspace::POSITIVE;
    }
    return Halfspace::ON_PLANE;
}

bool renderer::Frustrum::IsVisible(const math::Vector3f &point) const
{
    for (uint8_t i = 0; i < 6; ++i)
    {
        if (DistanceToPoint(mPlanes[i], point) < 0)
        {
            return false;
        }
    }
    return true;
}

bool renderer::Frustrum::IsVisible(const core::Box &box) const
{
    for (uint8_t i = 0; i < 6; i++)
    {
        uint8_t verticesOutOfFrustrum = 0, verticesInFrustrum = 0;
        for (uint8_t j = 0; j < 8 && (verticesInFrustrum == 0 || verticesOutOfFrustrum == 0); j++)
        {
            if (DistanceToPoint(mPlanes[i], box.vertices[j]) < 0)
            {
                ++verticesOutOfFrustrum;
            }
            else
            {
                ++verticesInFrustrum;
            }
        }
        if (verticesInFrustrum == 0)
        {
            return false;
        }
    }
    return true;
}

bool renderer::Frustrum::IsVisible(const core::AABB& aabb) const
{
    for (uint8_t i = 0; i < 6; ++i)
    {
        uint8_t verticesOutOfFrustrum = 0;
        verticesOutOfFrustrum += math::Vector4f(aabb.GetMin().X(), aabb.GetMin().Y(), aabb.GetMin().Z(), 1.0f).Dot(mPlanes[i]) < 0.0f ? 1 : 0;
        verticesOutOfFrustrum += math::Vector4f(aabb.GetMax().X(), aabb.GetMin().Y(), aabb.GetMin().Z(), 1.0f).Dot(mPlanes[i]) < 0.0f ? 1 : 0;
        verticesOutOfFrustrum += math::Vector4f(aabb.GetMin().X(), aabb.GetMax().Y(), aabb.GetMin().Z(), 1.0f).Dot(mPlanes[i]) < 0.0f ? 1 : 0;
        verticesOutOfFrustrum += math::Vector4f(aabb.GetMax().X(), aabb.GetMax().Y(), aabb.GetMin().Z(), 1.0f).Dot(mPlanes[i]) < 0.0f ? 1 : 0;
        verticesOutOfFrustrum += math::Vector4f(aabb.GetMin().X(), aabb.GetMax().Y(), aabb.GetMax().Z(), 1.0f).Dot(mPlanes[i]) < 0.0f ? 1 : 0;
        verticesOutOfFrustrum += math::Vector4f(aabb.GetMax().X(), aabb.GetMin().Y(), aabb.GetMax().Z(), 1.0f).Dot(mPlanes[i]) < 0.0f ? 1 : 0;
        verticesOutOfFrustrum += math::Vector4f(aabb.GetMin().X(), aabb.GetMax().Y(), aabb.GetMax().Z(), 1.0f).Dot(mPlanes[i]) < 0.0f ? 1 : 0;
        verticesOutOfFrustrum += math::Vector4f(aabb.GetMax().X(), aabb.GetMax().Y(), aabb.GetMax().Z(), 1.0f).Dot(mPlanes[i]) < 0.0f ? 1 : 0;
        if (verticesOutOfFrustrum == 8)
        {
            return false;
        }
    }
    return true;
}

void renderer::Frustrum::ExtractPlanes(const renderer::Camera &camera, bool normalize)
{
    const math::Matrix4x4& comboMatrix = camera.GetProjectionMatrix4x4() * camera.GetViewMatrix3x4();

    // Left clipping plane
    mPlanes[0].SetX(comboMatrix._41() + comboMatrix._11());
    mPlanes[0].SetY(comboMatrix._42() + comboMatrix._12());
    mPlanes[0].SetZ(comboMatrix._43() + comboMatrix._13());
    mPlanes[0].SetW(comboMatrix._44() + comboMatrix._14());
    // Right clipping plane
    mPlanes[1].SetX(comboMatrix._41() - comboMatrix._11());
    mPlanes[1].SetY(comboMatrix._42() - comboMatrix._12());
    mPlanes[1].SetZ(comboMatrix._43() - comboMatrix._13());
    mPlanes[1].SetW(comboMatrix._44() - comboMatrix._14());
    // Top clipping plane
    mPlanes[2].SetX(comboMatrix._41() - comboMatrix._21());
    mPlanes[2].SetY(comboMatrix._42() - comboMatrix._22());
    mPlanes[2].SetZ(comboMatrix._43() - comboMatrix._23());
    mPlanes[2].SetW(comboMatrix._44() - comboMatrix._24());
    // Bottom clipping plane
    mPlanes[3].SetX(comboMatrix._41() + comboMatrix._21());
    mPlanes[3].SetY(comboMatrix._42() + comboMatrix._22());
    mPlanes[3].SetZ(comboMatrix._43() + comboMatrix._23());
    mPlanes[3].SetW(comboMatrix._44() + comboMatrix._24());
    // Near clipping plane
    mPlanes[4].SetX(comboMatrix._41() + comboMatrix._31());
    mPlanes[4].SetY(comboMatrix._42() + comboMatrix._32());
    mPlanes[4].SetZ(comboMatrix._43() + comboMatrix._33());
    mPlanes[4].SetW(comboMatrix._44() + comboMatrix._34());
    // Far clipping plane
    mPlanes[5].SetX(comboMatrix._41() - comboMatrix._31());
    mPlanes[5].SetY(comboMatrix._42() - comboMatrix._32());
    mPlanes[5].SetZ(comboMatrix._43() - comboMatrix._33());
    mPlanes[5].SetW(comboMatrix._44() - comboMatrix._34());
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
