#pragma once

#include "renderer.h"
#include "chunkmanager.h"
#include "wii_pad.h"
#include "eventmanager.h"

namespace wiicraft {

class Player
{
public:
    Player(std::shared_ptr<renderer::Camera> playerCamera, std::shared_ptr<core::WiiPad> pad);
    ~Player();
    Player(const Player&) = delete;
    Player& operator = (const Player&) = delete;
    Player(Player&&) = delete;
    Player& operator = (Player&&) = delete;

    void OnRender3D(float deltaSeconds, renderer::Renderer& renderer, ChunkManager& world);
    void OnRender2D(float deltaSeconds, renderer::Renderer& renderer);
    inline double GetStance() const;
    inline const math::Vector3f& GetPosition() const;

    void DrawAABB(renderer::Renderer& renderer) const;
private:
    void OnPlayerSpawn(core::IEventDataPtr eventData);
    void OnSetPlayerPositionAndLook(core::IEventDataPtr eventData);
    void OnSendPlayerPosition(core::IEventDataPtr eventData);

    void SendPlayerPositionAndRotation() const;

private:
    const math::Vector3f mHitbox = {0.3f, .9f, 0.3f};
    const math::Vector3f mCameraOffset = {0.0f, 1.5f, 0.0f};
    math::Vector3f mPosition;
    std::unique_ptr<renderer::Image2D> mCrossHairImage;
    std::unique_ptr<renderer::Sprite> mCrossHairSprite;
    std::shared_ptr<core::WiiPad> mPad;
    std::shared_ptr<renderer::Camera> mCamera;
    double mStance;
    bool mOnGround;
    bool mSpawned;
};

inline double Player::GetStance() const
{
    return mStance;
}

inline const math::Vector3f &Player::GetPosition() const
{
    return mPosition;
}

}
