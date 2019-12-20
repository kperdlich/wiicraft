#pragma once

#include <array>
#include "renderer.h"
#include "chunkmanager.h"
#include "wii_pad.h"
#include "eventmanager.h"
#include "slotdata.h"

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
    void OnRender2D(float deltaSeconds, renderer::Renderer& renderer, ChunkManager &world);

    void DrawAABB(renderer::Renderer& renderer) const;
    inline double GetStance() const;
    inline const math::Vector3f& GetPosition() const;
    inline bool HasSpawned() const;

private:
    void OnPlayerSpawn(core::IEventDataPtr eventData);
    void OnSetPlayerPositionAndLook(core::IEventDataPtr eventData);
    void OnSendPlayerPosition(core::IEventDataPtr eventData);
    void OnUpdatePlayerListItem(core::IEventDataPtr eventData);
    void OnUpdatePlayerAbilities(core::IEventDataPtr eventData);
    void OnUpdateHotbarData(core::IEventDataPtr eventData);

    void SetActiveHotbarSlot(int8_t slot);
    void SendPlayerPositionAndRotation() const;

private:
    std::array<SlotData, 9> mHotbar;
    const math::Vector3f mHitbox = {0.3f, .9f, 0.3f};
    const math::Vector3f mCameraOffset = {0.0f, 1.62f, 0.0f};
    math::Vector3f mPosition;
    std::string mName;
    std::unique_ptr<renderer::DisplayList> mPlayerItemDisplayList;
    std::unique_ptr<renderer::Image2D> mCrossHairImage;
    std::unique_ptr<renderer::Sprite> mCrossHairSprite;
    std::unique_ptr<renderer::Image2D> mHotbarImage;
    std::unique_ptr<renderer::Sprite> mHotbarSprite;
    std::unique_ptr<renderer::Image2D> mHotbarIndexImage;
    std::unique_ptr<renderer::Sprite> mHotbarIndexSprite;
    std::unique_ptr<renderer::Image2D> mTerrainImage;
    std::unique_ptr<renderer::Sprite> mTerrainSprite;
    std::shared_ptr<core::WiiPad> mPad;
    std::shared_ptr<renderer::Camera> mCamera;
    double mStance;
    int16_t mPing;
    int8_t mCurrenHotbarIndex;
    bool mInvulnerability;
    bool mIsFlying;
    bool mCanFly;
    bool mInstantDestroy;
    bool mOnGround;
    bool mSpawned;
};

inline bool Player::HasSpawned() const
{
    return mSpawned;
}

inline double Player::GetStance() const
{
    return mStance;
}

inline const math::Vector3f &Player::GetPosition() const
{
    return mPosition;
}

}
