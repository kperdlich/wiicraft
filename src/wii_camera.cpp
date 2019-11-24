#include <math.h>
#include "geometry_data.h"
#include "mathhelper.h"
#include "camera.h"
#include "wii_defines.h"


renderer::Camera::Camera(const math::Vector3f &position,
                         const math::Vector3f &up,
                         const math::Vector3f &lookAt,
                         bool isPerspective)
    : mIsPerspective(isPerspective)
{
    float mtx[3][4];
    guVector vecPos = {position.X(), position.Y(), position.Z()};
    guVector vecCamUp = {up.X(), up.Y(), up.Z()};
    guVector vecLookAt = {lookAt.X(), lookAt.Y(), lookAt.Z()};
    guLookAt(mtx, &vecPos, &vecCamUp, &vecLookAt);
    mViewMatrix = mtx;
}

void renderer::Camera::SetFrustrum(float minDist, float maxDist, float fov, float aspectRatio)
{
    mFrustrumTop = minDist * std::tan(math::DegToRadians(fov)/2.0f);
    mFrustrumRight = mFrustrumTop * aspectRatio;
    mFrustrumBottom = -mFrustrumTop;
    mFrustrumLeft = -mFrustrumRight;
    mFrustrumNear = minDist;
    mFrustrumFar = maxDist;
}

void renderer::Camera::SetFrustrum(float top, float bottom, float left, float right, float near, float far)
{
    mFrustrumTop = top;
    mFrustrumBottom = bottom;
    mFrustrumLeft = left;
    mFrustrumRight = right;
    mFrustrumNear = near;
    mFrustrumFar = far;
}

void renderer::Camera::GenerateFrustrumPlanes(bool normalize)
{
    mFrustrum.ExtractPlanes(*this, normalize);
}

bool renderer::Camera::IsVisible(const math::Vector3f &point) const
{
    return mFrustrum.IsVisible(point);
}

bool renderer::Camera::IsVisible(const core::Box &box) const
{
    return mFrustrum.IsVisible(box);
}

bool renderer::Camera::IsVisible(const core::AABB &aabb) const
{
    return mFrustrum.IsVisible(aabb);
}

math::Matrix3x4 renderer::Camera::GetViewMatrix3x4() const
{    
    return mViewMatrix;
    /*return math::Matrix3x4(
                mRight.X(), mRight.Y(), mRight.Z(), -mRight.Dot(mPosition),
                mUp.X(), mUp.Y(), mUp.Z(), -mUp.Dot(mPosition),
                mLookAt.X(), mLookAt.Y(), mLookAt.Z(), -mLookAt.Dot(mPosition));*/

}

math::Matrix4x4 renderer::Camera::GetProjectionMatrix4x4() const
{
    Mtx44 mtx;
    if (mIsPerspective)
    {
        guFrustum(mtx, mFrustrumTop, mFrustrumBottom, mFrustrumLeft, mFrustrumRight, mFrustrumNear, mFrustrumFar);
    }
    else
    {
        guOrtho(mtx, mFrustrumTop, mFrustrumBottom, mFrustrumLeft, mFrustrumRight, mFrustrumNear, mFrustrumFar);
    }

    return mtx;
}

void renderer::Camera::Translate(float x, float y, float z)
{
    mViewMatrix.Translate(x, y, z);
}

void renderer::Camera::Rotate(const char axis, float degree)
{
    mViewMatrix.Rotate(axis, degree);
}
