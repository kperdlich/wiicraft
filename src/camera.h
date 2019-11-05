#pragma once

#include "vector3f.h"
#include "matrix3x4.h"
#include "matrix4x4.h"


namespace renderer {
class Camera {
public:
    Camera(const math::Vector3f& position,
           const math::Vector3f& up,
           const math::Vector3f& lookAt,
           bool isPerspective);
    ~Camera() = default;
    Camera(const Camera&) = delete;
    Camera(Camera&&) = delete;
    void operator=(const Camera&) = delete;
    void operator=(Camera&&) = delete;
    void SetFrustrum(float minDist, float maxDist, float fov, float aspectRatio);
    void SetFrustrum(float top, float bottom, float left, float right, float near, float far);    

    math::Matrix4x4 GetProjectionMatrix4x4() const;
    math::Matrix3x4 GetViewMatrix3x4() const;

    inline void SetPerspective(bool isPerspective);
    inline bool IsPerspective() const;
    inline float GetFrustrumNear() const;
    inline float GetFrustrumFar() const;
    inline float GetFrustrumTop() const;
    inline float GetFrustrumBottom() const;
    inline float GetFrustrumLeft() const;
    inline float GetFrustrumRight() const;

private:
    math::Vector3f mPosition;
    math::Vector3f mUp;
    math::Vector3f mLookAt;
    math::Vector3f mRight;
    float mFrustrumNear, mFrustrumFar, mFrustrumTop, mFrustrumBottom, mFrustrumLeft, mFrustrumRight;
    bool mIsPerspective;
};

inline void Camera::SetPerspective(bool isPerspective)
{
    mIsPerspective = isPerspective;
}

inline bool Camera::IsPerspective() const
{
    return mIsPerspective;
}

inline float Camera::GetFrustrumNear() const
{
    return mFrustrumNear;
}

inline float Camera::GetFrustrumFar() const
{
    return mFrustrumFar;
}

inline float Camera::GetFrustrumTop() const
{
    return mFrustrumTop;
}

inline float Camera::GetFrustrumBottom() const
{
    return mFrustrumBottom;
}

inline float Camera::GetFrustrumLeft() const
{
    return mFrustrumLeft;
}

inline float Camera::GetFrustrumRight() const
{
    return mFrustrumRight;
}

}
