#include "EntityManager.h"
#include "EventDataSpawnEntity.h"
#include "EventDataDestroyEntity.h"
#include "EventDataEntityMove.h"
#include "EventDataEntityRelativeMove.h"
#include "aabb.h"
#include "packet/PacketChatMessage.h"

wiicraft::Entity::Entity(int32_t id, const std::string &name, int32_t x, int32_t y, int32_t z, int16_t currentItem, int8_t yaw, int8_t pitch)
    : mName(name),
      mID(id),
      mX(x),
      mY(y),
      mZ(z),
      mCurrentIten(currentItem),
      mPitch(pitch),
      mYaw(yaw)
{
}

wiicraft::EntityManager::EntityManager()
{
    core::IEventManager::Get()->AddListener(fastdelegate::MakeDelegate(this, &EntityManager::OnEntitySpawn), EventDataSpawnEntity::EventType);
    core::IEventManager::Get()->AddListener(fastdelegate::MakeDelegate(this, &EntityManager::OnEntityRemove), EventDataDestroyEntity::EventType);
    core::IEventManager::Get()->AddListener(fastdelegate::MakeDelegate(this, &EntityManager::OnEntityMove), EventDataEntityMove::EventType);
    core::IEventManager::Get()->AddListener(fastdelegate::MakeDelegate(this, &EntityManager::OnEntityRelativeMove), EventDataEntityRelativeMove::EventType);
}

wiicraft::EntityManager::~EntityManager()
{
    core::IEventManager::Get()->RemoveListener(fastdelegate::MakeDelegate(this, &EntityManager::OnEntitySpawn), EventDataSpawnEntity::EventType);
    core::IEventManager::Get()->RemoveListener(fastdelegate::MakeDelegate(this, &EntityManager::OnEntityRemove), EventDataDestroyEntity::EventType);
    core::IEventManager::Get()->RemoveListener(fastdelegate::MakeDelegate(this, &EntityManager::OnEntityMove), EventDataEntityMove::EventType);
    core::IEventManager::Get()->RemoveListener(fastdelegate::MakeDelegate(this, &EntityManager::OnEntityRelativeMove), EventDataEntityMove::EventType);
}

void wiicraft::EntityManager::Render(renderer::Renderer &renderer)
{
    for (const auto& entity : mEntities)
    {
        const math::Vector3f entityAABBPos = {static_cast<float>(entity.second->GetX() / 32.0f), static_cast<float>(entity.second->GetY() / 32.0f), static_cast<float>(entity.second->GetZ() / 32.0f)};
        renderer.DrawAABB({entityAABBPos + math::Vector3f(0.0f, .9f, 0.0f), math::Vector3f(0.3f, .9f, 0.3f)}, renderer::ColorRGBA::RED);
    }
}

void wiicraft::EntityManager::OnEntitySpawn(core::IEventDataPtr eventData)
{
    std::shared_ptr<EventDataSpawnEntity> spawnEntity = std::static_pointer_cast<EventDataSpawnEntity>(eventData);
    mEntities[spawnEntity->GetEntity()->GetID()] = spawnEntity->GetEntity();
    PacketChatMessage msg("EntitySpawn");
    msg.Send();
}

void wiicraft::EntityManager::OnEntityRemove(core::IEventDataPtr eventData)
{
    std::shared_ptr<EventDataDestroyEntity> entityData = std::static_pointer_cast<EventDataDestroyEntity>(eventData);
    auto findIt = mEntities.find(entityData->GetEntityId());
    if (findIt != mEntities.end())
    {
        mEntities.erase(findIt);
        PacketChatMessage msg("EntityRemove");
        msg.Send();
    }
}

void wiicraft::EntityManager::OnEntityMove(core::IEventDataPtr eventData)
{
    std::shared_ptr<EventDataEntityMove> entityData = std::static_pointer_cast<EventDataEntityMove>(eventData);
    auto findIt = mEntities.find(entityData->GetEntityID());
    if (findIt != mEntities.end())
    {
        auto entity = findIt->second;
        entity->SetPosition(entityData->GetX(), entityData->GetY(), entityData->GetZ());
        entity->SetYaw(entityData->GetYaw());
        entity->SetPitch(entityData->GetPitch());
    }
}

void wiicraft::EntityManager::OnEntityRelativeMove(core::IEventDataPtr eventData)
{
    std::shared_ptr<EventDataEntityRelativeMove> entityData = std::static_pointer_cast<EventDataEntityRelativeMove>(eventData);
    auto findIt = mEntities.find(entityData->GetEntityID());
    if (findIt != mEntities.end())
    {
        findIt->second->UpdatePositionAndRotation(entityData->GetX(), entityData->GetY(), entityData->GetZ(), entityData->GetYaw(), entityData->GetPitch());
    }
}


