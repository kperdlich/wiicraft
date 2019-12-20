#include <sstream> // TODO remove
#include "player.h"
#include "camera.h"
#include "image2d.h"
#include "wii_sprite.h"
#include "crosshair_png.h"
#include "Hotbar_png.h"
#include "hobtbar_small_png.h"
#include "HotbarIndex_png.h"
#include "terraNew_png.h"
#include "eventmanager.h"
#include "PacketPlayerPosition.h"
#include "PacketPlayerPositionAndLook.h"
#include "PacketPlayerDigging.h"
#include "PacketPlayerBlockPlacement.h"
#include "PacketHeldItemChange.h"
#include "EventDataSpawnPlayer.h"
#include "EventDataSetPlayerPositionAndLook.h"
#include "EventDataSendPlayerPosition.h"
#include "EventDataChangeBlock.h"
#include "EventDataUpdatePlayerListItem.h"
#include "EventDataUpdatePlayerAbilities.h"
#include "EventDataUpdatePlayerHotbarData.h"

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
      mPing(0),
      mCurrenHotbarIndex(0),
      mInvulnerability(false),
      mIsFlying(false),
      mCanFly(false),
      mInstantDestroy(false),
      mOnGround(false),
      mSpawned(false)

{
    mCrossHairImage = std::make_unique<renderer::Image2D>(crosshair_png);
    mCrossHairSprite = std::make_unique<renderer::Sprite>(*mCrossHairImage);
    mCrossHairSprite->SetScaleX(0.01f);
    mCrossHairSprite->SetScaleY(0.01f);

    mHotbarImage = std::make_unique<renderer::Image2D>(hobtbar_small_png);
    mHotbarSprite = std::make_unique<renderer::Sprite>(*mHotbarImage);

    mHotbarIndexImage = std::make_unique<renderer::Image2D>(HotbarIndex_png);
    mHotbarIndexSprite = std::make_unique<renderer::Sprite>(*mHotbarIndexImage);

    mPlayerItemDisplayList = std::make_unique<renderer::DisplayList>();
    mTerrainImage = std::make_unique<renderer::Image2D>(terraNew_png);
    mTerrainSprite = std::make_unique<renderer::Sprite>(*mTerrainImage);

    mName = "DaeFennek"; // TODO read from config

    core::IEventManager::Get()->AddListener(fastdelegate::MakeDelegate(this, &Player::OnPlayerSpawn), EventDataSpawnPlayer::EventType);
    core::IEventManager::Get()->AddListener(fastdelegate::MakeDelegate(this, &Player::OnSetPlayerPositionAndLook), EventDataSetPlayerPositionAndLook::EventType);
    core::IEventManager::Get()->AddListener(fastdelegate::MakeDelegate(this, &Player::OnSendPlayerPosition), EventDataSendPlayerPosition::EventType);
    core::IEventManager::Get()->AddListener(fastdelegate::MakeDelegate(this, &Player::OnUpdatePlayerListItem), EventDataUpdatePlayerListItem::EventType);
    core::IEventManager::Get()->AddListener(fastdelegate::MakeDelegate(this, &Player::OnUpdatePlayerAbilities), EventDataUpdatePlayerAbilities::EventType);
    core::IEventManager::Get()->AddListener(fastdelegate::MakeDelegate(this, &Player::OnUpdateHotbarData), EventDataUpdatePlayerHotbarData::EventType);
}


wiicraft::Player::~Player()
{
    core::IEventManager::Get()->RemoveListener(fastdelegate::MakeDelegate(this, &Player::OnPlayerSpawn), EventDataSpawnPlayer::EventType);
    core::IEventManager::Get()->RemoveListener(fastdelegate::MakeDelegate(this, &Player::OnSetPlayerPositionAndLook), EventDataSetPlayerPositionAndLook::EventType);
    core::IEventManager::Get()->RemoveListener(fastdelegate::MakeDelegate(this, &Player::OnSendPlayerPosition), EventDataSendPlayerPosition::EventType);
    core::IEventManager::Get()->RemoveListener(fastdelegate::MakeDelegate(this, &Player::OnUpdatePlayerListItem), EventDataUpdatePlayerListItem::EventType);
    core::IEventManager::Get()->RemoveListener(fastdelegate::MakeDelegate(this, &Player::OnUpdatePlayerAbilities), EventDataUpdatePlayerAbilities::EventType);
    core::IEventManager::Get()->RemoveListener(fastdelegate::MakeDelegate(this, &Player::OnUpdateHotbarData), EventDataUpdatePlayerHotbarData::EventType);
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

    bool collision = false;
    math::Vector3f playerHitBoxPosition = mPosition + math::Vector3f(0.0f, mHitbox.Y(), 0.0f);
    std::vector<core::AABB> collidableAABBsAroundPlayer = world.GetCollidableBlockAABBsAround(playerHitBoxPosition); // Around center of hitbox
    if (mPad->GetNunchukAngleY() > 0.0f)
    {
        core::AABB playerAABB(playerHitBoxPosition + (mCamera->Forward() * 0.1f), mHitbox);
         for (const core::AABB& aabb : collidableAABBsAroundPlayer)
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
        core::AABB playerAABB(playerHitBoxPosition + (mCamera->Forward() * -.1f) , mHitbox);
        for (const core::AABB& aabb : collidableAABBsAroundPlayer)
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
        core::AABB playerAABB(playerHitBoxPosition + (mCamera->Right() * -.1f) , mHitbox);
        for (const core::AABB& aabb : collidableAABBsAroundPlayer)
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
        core::AABB playerAABB(playerHitBoxPosition + (mCamera->Right() * .1f) , mHitbox);
        for (const core::AABB& aabb : collidableAABBsAroundPlayer)
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
                // Only enough for creative mode
                PacketPlayerDigging packetStart(focusedBlock.Entity.GetCenter().X(), focusedBlock.Entity.GetCenter().Y(), focusedBlock.Entity.GetCenter().Z(),
                                           0, focusedBlock.Normal);
                packetStart.Send();
            }
        }
        if (mPad->ButtonsDown() & WPAD_BUTTON_A)
        {
            const math::Vector3f newBlockPosition = focusedBlock.Entity.GetCenter();
            const wiicraft::ChunkPosition chunkPosition = wiicraft::ChunkSection::WorldPositionToChunkPosition(newBlockPosition);
            //renderer.DrawAABB({newBlockPosition, {wiicraft::BlockManager::BLOCK_HALF_SIZE + 0.1f, wiicraft::BlockManager::BLOCK_HALF_SIZE + 0.1f, wiicraft::BlockManager::BLOCK_HALF_SIZE + 0.1f}}, renderer::ColorRGBA::RED);
            const auto chunkSection = world.GetChunk(chunkPosition);
            if (chunkSection)
            {                                
                PacketPlayerBlockPlacement packet(
                            focusedBlock.Entity.GetCenter().X(), focusedBlock.Entity.GetCenter().Y(), focusedBlock.Entity.GetCenter().Z(),
                            focusedBlock.Normal, mHotbar[mCurrenHotbarIndex]);
                packet.Send();               
            }
        }
    }
    else
    {
        mCrossHairSprite->SetColor(renderer::ColorRGBA::WHITE);
    }


    if (mPad->ButtonsDown() & WPAD_BUTTON_LEFT)
    {
        SetActiveHotbarSlot(mCurrenHotbarIndex -1);
    }
    if (mPad->ButtonsDown() & WPAD_BUTTON_RIGHT)
    {
        SetActiveHotbarSlot(mCurrenHotbarIndex + 1);
    }


    math::Matrix3x4 playerCubeTranslation, playerCubeScale, playerCubeRotation;
    playerCubeRotation.SetIdentity();
    playerCubeRotation.Rotate('Y', 49.0f);
    playerCubeScale.SetIdentity();
    playerCubeScale.Scale(0.3f, 0.3f, 0.3f);
    playerCubeTranslation.SetIdentity();
    playerCubeTranslation.Translate(0.35f, -0.42f, -0.6f);
    renderer.LoadModelViewMatrix(mCamera->GetViewMatrix3x4() * mCamera->GetViewMatrix3x4().Inverse() * playerCubeTranslation * playerCubeRotation * playerCubeScale);

    if (mHotbar[mCurrenHotbarIndex].ItemID > 0)
    {
        if (mPlayerItemDisplayList->GetBufferSize() == 0)
        {
            world.GetBlockManager().GenerateMultiTexturedBlockMesh(static_cast<BlockType>(mHotbar[mCurrenHotbarIndex].ItemID), *mPlayerItemDisplayList);
        }

        renderer.SetZModeEnabled(false);
        mPlayerItemDisplayList->Render();
        renderer.SetZModeEnabled(true);
    }

    if (mSpawned)
    {
        SendPlayerPositionAndRotation();
    }
}

void wiicraft::Player::OnRender2D(float deltaSeconds, renderer::Renderer &renderer, ChunkManager& world)
{
    //mCrossHairSprite->SetPosX(renderer.GetWidth() * 0.5f);
    //mCrossHairSprite->SetPosY(renderer.GetHeight() * 0.5f);        
    mHotbarSprite->SetPosX(renderer.GetWidth() * .5f);
    mHotbarSprite->SetPosY(renderer.GetHeight() - (mHotbarImage->Height() * .5f));
    mHotbarIndexSprite->SetPosX((mHotbarSprite->GetX() - (mHotbarImage->Width() * .5f) + (mHotbarIndexSprite->Width() * .5f) - 2
                                 + mCurrenHotbarIndex * 40.0f));
    mHotbarIndexSprite->SetPosY(mHotbarSprite->GetY() -2);
    renderer.SetZModeEnabled(false);
    renderer.Draw(*mHotbarSprite);   
    renderer.SetZModeEnabled(true);

    for (uint8_t i = 0; i < mHotbar.size(); ++i)
    {
        if (mHotbar[i].ItemID >= 0)
        {
            const auto& block = world.GetBlockManager().GetBlock(static_cast<BlockType>(mHotbar[i].ItemID));
            int32_t x = mHotbarSprite->GetX() - mHotbarSprite->Width() * .5f + (32 * .5f) + 10;
            int32_t y = mHotbarSprite->GetY();
            renderer.DrawSpriteSheet(x + (32*i) + (i*7), y, *mTerrainSprite, block->GetTextureIndexForSide(BlockFace::Back),
                                     32, 32 ,32 ,32);
        }
    }

    renderer.SetZModeEnabled(false);
    renderer.Draw(*mHotbarIndexSprite);
    renderer.Draw(*mCrossHairSprite);
    renderer.SetZModeEnabled(true);

    std::wstringstream ping;
    ping << "Ping: " << mPing << "ms";
    renderer.DrawText(30, 30, ping.str(), renderer::ColorRGBA::WHITE);

    std::wstringstream blockPos;
    blockPos << "Block: XYZ: " << sfocusedBlock.Entity.GetCenter().X()
             << "/" << sfocusedBlock.Entity.GetCenter().Y()
             << "/" << sfocusedBlock.Entity.GetCenter().Z();
    renderer.DrawText(30, 50, blockPos.str(), renderer::ColorRGBA::WHITE);

    std::wstringstream playerAbilities;
    playerAbilities << "IV:" << mInvulnerability << " / IF:" << mIsFlying << " / CF:" << mCanFly
                    << " / ID:" << mInstantDestroy;
    renderer.DrawText(30, 70, playerAbilities.str(), renderer::ColorRGBA::WHITE);
}

void wiicraft::Player::DrawAABB(renderer::Renderer &renderer) const
{
    renderer.LoadModelViewMatrix(renderer.GetCamera()->GetViewMatrix3x4() * math::Matrix3x4::Identity());
    renderer.DrawAABB({mPosition + math::Vector3f(0.0f, mHitbox.Y(), 0.0f), mHitbox}, renderer::ColorRGBA::BLUE);
}

void wiicraft::Player::SetActiveHotbarSlot(int8_t slot)
{
    const int8_t newHotbarIndex = slot % static_cast<int8_t>(mHotbar.size());
    if (newHotbarIndex != mCurrenHotbarIndex)
    {
        mPlayerItemDisplayList->Clear();
        mCurrenHotbarIndex = newHotbarIndex;
        if (mCurrenHotbarIndex < 0)
        {
            mCurrenHotbarIndex = static_cast<int8_t>(mHotbar.size()) - 1;
        }
        PacketHeldItemChange packet(mCurrenHotbarIndex);
        packet.Send();
    }
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

void wiicraft::Player::OnUpdatePlayerListItem(core::IEventDataPtr eventData)
{
    std::shared_ptr<EventDataUpdatePlayerListItem> playerListItem = std::static_pointer_cast<EventDataUpdatePlayerListItem>(eventData);
    if (playerListItem->GetPlayerName() == mName)
    {
        mPing = playerListItem->GetPing();
    }
}

void wiicraft::Player::OnUpdatePlayerAbilities(core::IEventDataPtr eventData)
{
    std::shared_ptr<EventDataUpdatePlayerAbilities> playerAbilities = std::static_pointer_cast<EventDataUpdatePlayerAbilities>(eventData);
    mInvulnerability = playerAbilities->GetInvulnerability();
    mIsFlying = playerAbilities->GetIsFlying();
    mCanFly = playerAbilities->GetCanFlying();
    mInstantDestroy = playerAbilities->GetInstantDestroy();
}

void wiicraft::Player::OnUpdateHotbarData(core::IEventDataPtr eventData)
{
    std::shared_ptr<EventDataUpdatePlayerHotbarData> hotbarData = std::static_pointer_cast<EventDataUpdatePlayerHotbarData>(eventData);
    if (hotbarData->GetWindowId() == -1 && hotbarData->GetSlot() == -1)
    {
        ASSERT(mCurrenHotbarIndex >= 0 && mCurrenHotbarIndex < mHotbar.size());
        mHotbar[mCurrenHotbarIndex] = hotbarData->GetSlotData();
        mPlayerItemDisplayList->Clear();
    }
    else
    {
        ASSERT(hotbarData->GetWindowId() == 0); // So far only support player hotbar inventory
        ASSERT(hotbarData->GetSlot() >= 36 && hotbarData->GetSlot() <= 44); // hotbar slots are between 36 and 44
        mHotbar[hotbarData->GetSlot() - 36] = hotbarData->GetSlotData();
    }
}

void wiicraft::Player::SendPlayerPositionAndRotation() const
{
    PacketPlayerPositionAndLook packet(mPosition.X(), mPosition.Y(), mPosition.Z(), mCamera->GetYaw(), -mCamera->GetPitch(), mStance, mOnGround);
    packet.Send();
}

