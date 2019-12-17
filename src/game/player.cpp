#include <sstream> // TODO remove
#include "player.h"
#include "camera.h"
#include "image2d.h"
#include "wii_sprite.h"
#include "crosshair_png.h"
#include "eventmanager.h"
#include "PacketPlayerPosition.h"
#include "PacketPlayerPositionAndLook.h"
#include "PacketPlayerDigging.h"
#include "PacketKeepAlive.h"
#include "EventDataSpawnPlayer.h"
#include "EventDataSetPlayerPositionAndLook.h"
#include "EventDataSendPlayerPosition.h"
#include "EventDataRemoveBlock.h"

constexpr float ROTATION_SPEED = 70.0f;
constexpr float MOVEMENT_SPEED = 4.0f;
constexpr float PITCH_MAX = 90.0f;
constexpr float MAX_JUMP_HEIGHT = 1.3f;
constexpr float PLAYER_GRAVITY = 4.0f;

wiicraft::Player::Player(std::shared_ptr<renderer::Camera> playerCamera, std::shared_ptr<core::WiiPad> pad)
    : mPosition(playerCamera->Position() - mCameraOffset),
      mPad(pad),
      mCamera(playerCamera),
      mStance(0.0),
      mOnGround(false),
      mSpawned(false)
{
    mCrossHairImage = std::make_unique<renderer::Image2D>(crosshair_png);
    mCrossHairSprite = std::make_unique<renderer::Sprite>(*mCrossHairImage);

    core::IEventManager::Get()->AddListener(fastdelegate::MakeDelegate(this, &Player::OnPlayerSpawn), EventDataSpawnPlayer::EventType);
    core::IEventManager::Get()->AddListener(fastdelegate::MakeDelegate(this, &Player::OnSetPlayerPositionAndLook), EventDataSetPlayerPositionAndLook::EventType);
    core::IEventManager::Get()->AddListener(fastdelegate::MakeDelegate(this, &Player::OnSendPlayerPosition), EventDataSendPlayerPosition::EventType);
}

wiicraft::Player::~Player()
{
    core::IEventManager::Get()->RemoveListener(fastdelegate::MakeDelegate(this, &Player::OnPlayerSpawn), EventDataSpawnPlayer::EventType);
    core::IEventManager::Get()->RemoveListener(fastdelegate::MakeDelegate(this, &Player::OnSetPlayerPositionAndLook), EventDataSetPlayerPositionAndLook::EventType);
    core::IEventManager::Get()->RemoveListener(fastdelegate::MakeDelegate(this, &Player::OnSendPlayerPosition), EventDataSendPlayerPosition::EventType);
}

static core::RayHitResult sfocusedBlock;
void wiicraft::Player::OnRender3D(float deltaSeconds, renderer::Renderer &renderer, ChunkManager& world)
{
    if (mPad->GetY() <= 15.0f)
    {
        mCamera->Rotate(0.0f, ROTATION_SPEED * deltaSeconds);
    }
    else if (mPad->GetY() >= renderer.GetHeight() - 45.0f)
    {
         mCamera->Rotate(0.0f, -ROTATION_SPEED * deltaSeconds);
    }

    if (mPad->GetX() >= renderer.GetWidth() - 120.0f)
    {
         mCamera->Rotate(ROTATION_SPEED * deltaSeconds, 0.0f);
    }
    else if (mPad->GetX() <= 120.0f)
    {
         mCamera->Rotate(-ROTATION_SPEED * deltaSeconds, 0.0f);
    }

    mCrossHairSprite->SetPosX(mPad->GetX());
    mCrossHairSprite->SetPosY(mPad->GetY());

    std::vector<core::AABB> aabbsCollidedWithPlayer;

    math::Vector3f playerHitBoxPosition = mPosition + math::Vector3f(0.0f, mHitbox.Y(), 0.0f);
    std::vector<core::AABB> aabbsAroundPlayer = world.GetBlockAABBsAround(playerHitBoxPosition); // Around center of hitbox
    if (mPad->GetNunchukAngleY() > 0.0f)
    {
        core::AABB playerAABB(playerHitBoxPosition + mCamera->Forward(), mHitbox);
         bool collision = false;
         for (const core::AABB& aabb : aabbsAroundPlayer)
         {
             if (playerAABB.CoolidesWith(aabb))
             {
                 aabbsCollidedWithPlayer.push_back(aabb);
                 collision = true;
                 break;
             }
         }
       if (!collision)
          mCamera->Move(renderer::CameraMovementDirection::FORWARD, MOVEMENT_SPEED * deltaSeconds);
    }

    if (mPad->GetNunchukAngleY() < 0.0f)
    {
        core::AABB playerAABB(playerHitBoxPosition + (mCamera->Forward() * -1.0f) , mHitbox);
        bool collision = false;
        for (const core::AABB& aabb : aabbsAroundPlayer)
        {
            if (playerAABB.CoolidesWith(aabb))
            {
                aabbsCollidedWithPlayer.push_back(aabb);
                collision = true;
                break;
            }
        }
       if (!collision)
             mCamera->Move(renderer::CameraMovementDirection::BACKWARD, MOVEMENT_SPEED * deltaSeconds);
    }

    if (mPad->GetNunchukAngleX() < 0.0f)
    {
        core::AABB playerAABB(playerHitBoxPosition + (mCamera->Right() * -1.0f) , mHitbox);
        bool collision = false;
        for (const core::AABB& aabb : aabbsAroundPlayer)
        {
            if (playerAABB.CoolidesWith(aabb))
            {
                aabbsCollidedWithPlayer.push_back(aabb);
                collision = true;
                break;
            }
        }
       if (!collision)
             mCamera->Move(renderer::CameraMovementDirection::LEFT, MOVEMENT_SPEED * deltaSeconds);
    }

    if (mPad->GetNunchukAngleX() > 0.0f)
    {
        core::AABB playerAABB(playerHitBoxPosition + (mCamera->Right()) , mHitbox);
        bool collision = false;
        for (const core::AABB& aabb : aabbsAroundPlayer)
        {
            if (playerAABB.CoolidesWith(aabb))
            {
                aabbsCollidedWithPlayer.push_back(aabb);
                collision = true;
                break;
            }
        }
       if (!collision)
             mCamera->Move(renderer::CameraMovementDirection::RIGHT, MOVEMENT_SPEED * deltaSeconds);
    }

    for (const auto& eAABBS : aabbsAroundPlayer)
    {
        renderer.DrawAABB(eAABBS, renderer::ColorRGBA::WHITE);
    }

    for (const auto& eAABBS : aabbsCollidedWithPlayer)
    {
        renderer.DrawAABB(eAABBS, renderer::ColorRGBA::RED);
    }

    mPosition = mCamera->Position() - mCameraOffset;
    mStance = mPosition.Y() + 1.62f;

    core::RayHitResult focusedBlock;
    bool hasBlockInFocus = world.Raycast(mCamera->Position(), mCamera->Forward(),
                                         10.0f, focusedBlock);
    sfocusedBlock = focusedBlock;
    if (hasBlockInFocus)
    {
        mCrossHairSprite->SetColor(renderer::ColorRGBA::RED);
        renderer.SetLineWidth(26);
        renderer.LoadModelViewMatrix(mCamera->GetViewMatrix3x4() * math::Matrix3x4::Identity());
        renderer.DrawAABB(focusedBlock.Entity, renderer::ColorRGBA::BLACK);
        renderer.DrawRay(focusedBlock.Entity.GetCenter(), focusedBlock.Normal, renderer::ColorRGBA::GREEN);
        renderer.SetLineWidth(12);
        if (mPad->ButtonsDown() & WPAD_BUTTON_B)
        {
            const wiicraft::ChunkPosition chunkPosition = wiicraft::ChunkSection::WorldPositionToChunkPosition(focusedBlock.Entity.GetCenter());
            const auto chunkSection = world.GetChunk(chunkPosition);
            if (chunkSection)
            {
                int8_t face;
                if (focusedBlock.Normal.Y() < 0)
                    face = 0;
                else if (focusedBlock.Normal.Y() > 0)
                    face = 1;
                else if(focusedBlock.Normal.Z() < 0)
                    face = 2;
                else if (focusedBlock.Normal.Z() > 0)
                    face = 3;
                else if (focusedBlock.Normal.X() > 0)
                    face = 4;
                else if(focusedBlock.Normal.X() < 0)
                    face = 5;

                PacketPlayerDigging packetStart(focusedBlock.Entity.GetCenter().X(), focusedBlock.Entity.GetCenter().Y(), focusedBlock.Entity.GetCenter().Z(),
                                           0, face);
                packetStart.Send();
                PacketPlayerDigging packetEnd(focusedBlock.Entity.GetCenter().X(), focusedBlock.Entity.GetCenter().Y(), focusedBlock.Entity.GetCenter().Z(),
                                           2, face);
                packetEnd.Send();



                //chunkSection->SetBlock(wiicraft::ChunkSection::BlockWorldPositionToLocalChunkPosition(focusedBlock.Entity.GetCenter()),
                //                       wiicraft::BlockType::AIR);
            }
        }
        if (mPad->ButtonsDown() & WPAD_BUTTON_A)
        {
            const math::Vector3f newBlockPosition = focusedBlock.Entity.GetCenter() + focusedBlock.Normal;
            const wiicraft::ChunkPosition chunkPosition = wiicraft::ChunkSection::WorldPositionToChunkPosition(newBlockPosition);
            renderer.DrawAABB({newBlockPosition, {wiicraft::ChunkSection::BLOCK_HALF_SIZE + 0.1f, wiicraft::ChunkSection::BLOCK_HALF_SIZE + 0.1f, wiicraft::ChunkSection::BLOCK_HALF_SIZE + 0.1f}}, renderer::ColorRGBA::RED);
            const auto chunkSection = world.GetChunk(chunkPosition);
            if (chunkSection)
            {                
                chunkSection->SetBlock(wiicraft::ChunkSection::BlockWorldPositionToLocalChunkPosition(newBlockPosition),
                                       wiicraft::BlockType::DIRT);
            }
        }
    }
    else
    {
        mCrossHairSprite->SetColor(renderer::ColorRGBA::WHITE);
    }


    math::Matrix3x4 playerCubeTranslation, playerCubeScale, playerCubeRotation;
    playerCubeRotation.SetIdentity();
    playerCubeRotation.Rotate('Y', 49.0f);
    playerCubeScale.SetIdentity();
    playerCubeScale.Scale(0.3f, 0.3f, 0.3f);
    playerCubeTranslation.SetIdentity();
    playerCubeTranslation.Translate(0.35f, -0.42f, -0.6f);
    renderer.LoadModelViewMatrix(mCamera->GetViewMatrix3x4() * mCamera->GetViewMatrix3x4().Inverse() * playerCubeTranslation * playerCubeRotation * playerCubeScale);
    renderer.SetZModeEnabled(false);
    //renderer.Draw(cube); TODO Draw
    renderer.SetZModeEnabled(true);

    if (mSpawned)
    {
        SendPlayerPositionAndRotation();
    }
}

void wiicraft::Player::OnRender2D(float deltaSeconds, renderer::Renderer &renderer)
{
    //mCrossHairSprite->SetPosX(renderer.GetWidth() * 0.5f);
    //mCrossHairSprite->SetPosY(renderer.GetHeight() * 0.5f);
    mCrossHairSprite->SetScaleX(0.01f);
    mCrossHairSprite->SetScaleY(0.01f);
    renderer.Draw(*mCrossHairSprite);

    std::wstringstream blockPos;
    blockPos << "Block: X: " << sfocusedBlock.Entity.GetCenter().X();
    blockPos << " Y: " << sfocusedBlock.Entity.GetCenter().Y();
    blockPos << " Z: " << sfocusedBlock.Entity.GetCenter().Z();
    renderer.LoadModelViewMatrix(renderer.GetCamera()->GetViewMatrix3x4());
    renderer.DrawText(200, 200, blockPos.str(), renderer::ColorRGBA::BLUE);
}

void wiicraft::Player::DrawAABB(renderer::Renderer &renderer) const
{
    renderer.LoadModelViewMatrix(renderer.GetCamera()->GetViewMatrix3x4() * math::Matrix3x4::Identity());
    renderer.DrawAABB({mPosition + math::Vector3f(0.0f, mHitbox.Y(), 0.0f), mHitbox}, renderer::ColorRGBA::BLUE);
}

void wiicraft::Player::OnPlayerSpawn(core::IEventDataPtr eventData)
{
    std::shared_ptr<EventDataSpawnPlayer> playerSpawnEventData = std::static_pointer_cast<EventDataSpawnPlayer>(eventData);
    mPosition = playerSpawnEventData->GetSpawnPosition();
    mCamera->SetPosition(mPosition + mCameraOffset);
    mSpawned = true;
}

void wiicraft::Player::OnSetPlayerPositionAndLook(core::IEventDataPtr eventData)
{
    std::shared_ptr<EventDataSetPlayerPositionAndLook> playerPositionAndLook = std::static_pointer_cast<EventDataSetPlayerPositionAndLook>(eventData);
    mPosition = playerPositionAndLook->GetPosition();
    mCamera->SetPosition(mPosition + mCameraOffset);
    mCamera->SetYaw(playerPositionAndLook->GetYaw());
    mCamera->SetPitch(playerPositionAndLook->GetPitch());
    mOnGround = playerPositionAndLook->GetOnGround();
    mStance = playerPositionAndLook->GetStance();
}

void wiicraft::Player::OnSendPlayerPosition(core::IEventDataPtr eventData)
{
    PacketPlayerPosition playerPositionPacket(mPosition.X(), mPosition.Y(), mPosition.Z(), mStance, mOnGround);
    playerPositionPacket.Send();
}

void wiicraft::Player::SendPlayerPositionAndRotation() const
{
    PacketPlayerPositionAndLook packet(mPosition.X(), mPosition.Y(), mPosition.Z(), mCamera->GetYaw(), -mCamera->GetPitch(), mStance, mOnGround);
    packet.Send();
}


