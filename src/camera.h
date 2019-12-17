#pragma once

#include "vector3f.h"
#include "matrix3x4.h"
#include "matrix4x4.h"
#include "frustrum.h"


namespace renderer {

enum class CameraMovementDirection : char
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera {
public:
    Camera(const math::Vector3f& position,
           const math::Vector3f& worldUp,
           const math::Vector3f& lookAt,
           bool isPerspective);
    ~Camera() = default;
    Camera(const Camera&) = delete;
    Camera(Camera&&) = delete;
    Camera& operator=(const Camera&) = delete;
    Camera& operator=(Camera&&) = delete;

    void SetFrustrum(float minDist, float maxDist, float fov, float aspectRatio);
    void SetFrustrum(float top, float bottom, float left, float right, float near, float far);    

    math::Vector3f ScreenSpaceToWorldSpace(float posX, float posY, float screenWidth, float screenHeight) const;
    void GenerateFrustrumPlanes(bool normalize);
    bool IsVisible(const math::Vector3f &point) const;
    bool IsVisible(const core::Box& box) const;
    bool IsVisible(const core::AABB& aabb) const;

    math::Matrix3x4 GetViewMatrix3x4() const;
    math::Matrix4x4 GetProjectionMatrix4x4() const;

    void Move(const CameraMovementDirection& direction, float scale);
    void Rotate(float x, float y);    
    void SetYaw(float yaw);
    void SetPitch(float pitch);

    inline void SetPosition(const math::Vector3f position);
    inline bool IsPerspective() const;
    inline float GetFrustrumNear() const;
    inline float GetFrustrumFar() const;
    inline float GetFrustrumTop() const;
    inline float GetFrustrumBottom() const;
    inline float GetFrustrumLeft() const;
    inline float GetFrustrumRight() const;
    inline float GetYaw() const;
    inline float GetPitch() const;
    inline const math::Vector3f& Position() const;
    inline const math::Vector3f& Forward() const;
    inline const math::Vector3f& Right() const;

private:
    void UpdateCameraVectors();

private:
    renderer::Frustrum mFrustrum; // For Frustrum Culling
    math::Vector3f mPosition;
    math::Vector3f mUp;
    math::Vector3f mWorldUp;
    math::Vector3f mLookAt;
    math::Vector3f mRight;
    float mFrustrumNear, mFrustrumFar, mFrustrumTop, mFrustrumBottom, mFrustrumLeft, mFrustrumRight;
    float mYaw, mPitch;
    bool mIsPerspective;
};

inline void Camera::SetPosition(const math::Vector3f position)
{
    mPosition = position;
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

inline float Camera::GetYaw() const
{
    return mYaw;
}

inline float Camera::GetPitch() const
{
   return mPitch;
}

inline const math::Vector3f& Camera::Position() const
{
    return mPosition;
}

inline const math::Vector3f &Camera::Forward() const
{
    return mLookAt;
}

inline const math::Vector3f &Camera::Right() const
{
    return mRight;
}

}
