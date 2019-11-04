#include <math.h>
#include "mathhelper.h"
#include "camera.h"
#include "wiidefines.h"


renderer::Camera::Camera(const math::Vector3f &position,
                         const math::Vector3f &up,
                         const math::Vector3f &lookAt,
                         bool isPerspective)
    : mPosition(position),
      mUp(up),
      mLookAt(lookAt),
      mIsPerspective(isPerspective)
{
    mRight = position.Cross(up);
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

math::Matrix4x4 renderer::Camera::GetProjectionMatrix4x4() const
{
    float mtx[4][4];
    if (mIsPerspective)
    {
        /*return math::Matrix4x4(
                    2*mFrustrumNear/(mFrustrumRight-mFrustrumLeft), 0, 0, -mFrustrumNear*(mFrustrumRight+mFrustrumLeft)/(mFrustrumRight-mFrustrumLeft),
                    0, 2*mFrustrumNear/(mFrustrumTop-mFrustrumBottom), 0, -mFrustrumNear*(mFrustrumTop+mFrustrumBottom)/(mFrustrumTop-mFrustrumBottom),
                    0, 0, -(mFrustrumFar+mFrustrumNear)/(mFrustrumFar-mFrustrumNear), 2*mFrustrumFar*mFrustrumNear/(mFrustrumNear-mFrustrumFar),
                    0, 0, -1, 0);*/

        guFrustum(mtx, mFrustrumTop, mFrustrumBottom, mFrustrumLeft, mFrustrumRight, mFrustrumNear, mFrustrumFar);
    }
    else
    {
        guOrtho(mtx, mFrustrumTop, mFrustrumBottom, mFrustrumLeft, mFrustrumRight, mFrustrumNear, mFrustrumFar);
    }
    return math::Matrix4x4(mtx);
}

math::Matrix3x4 renderer::Camera::GetViewMatrix3x4() const
{
    return math::Matrix3x4(
                mRight.X(), mRight.Y(), mRight.Z(), -mRight.Dot(mPosition),
                mUp.X(), mUp.Y(), mUp.Z(), -mUp.Dot(mPosition),
                mLookAt.X(), mLookAt.Y(), mLookAt.Z(), -mLookAt.Dot(mPosition));

}
