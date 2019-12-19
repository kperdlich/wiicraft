#include "EntityManager.h"
#include "EventDataSpawnPlayerEntity.h"
#include "EventDataSpawnItemEntity.h"
#include "EventDataDestroyEntity.h"
#include "EventDataEntityMove.h"
#include "EventDataEntityRelativeMove.h"
#include "aabb.h"
#include "packet/PacketChatMessage.h"

wiicraft::EntityManager::EntityManager()
{
    core::IEventManager::Get()->AddListener(fastdelegate::MakeDelegate(this, &EntityManager::OnPlayerEntitySpawn), EventDataSpawnPlayerEntity::EventType);
    core::IEventManager::Get()->AddListener(fastdelegate::MakeDelegate(this, &EntityManager::OnItemEntitySpawn), EventDataSpawnItemEntity::EventType);
    core::IEventManager::Get()->AddListener(fastdelegate::MakeDelegate(this, &EntityManager::OnEntityRemove), EventDataDestroyEntity::EventType);
    core::IEventManager::Get()->AddListener(fastdelegate::MakeDelegate(this, &EntityManager::OnEntityMove), EventDataEntityMove::EventType);
    core::IEventManager::Get()->AddListener(fastdelegate::MakeDelegate(this, &EntityManager::OnEntityRelativeMove), EventDataEntityRelativeMove::EventType);
}

wiicraft::EntityManager::~EntityManager()
{
    core::IEventManager::Get()->RemoveListener(fastdelegate::MakeDelegate(this, &EntityManager::OnPlayerEntitySpawn), EventDataSpawnPlayerEntity::EventType);
    core::IEventManager::Get()->RemoveListener(fastdelegate::MakeDelegate(this, &EntityManager::OnItemEntitySpawn), EventDataSpawnItemEntity::EventType);
    core::IEventManager::Get()->RemoveListener(fastdelegate::MakeDelegate(this, &EntityManager::OnEntityRemove), EventDataDestroyEntity::EventType);
    core::IEventManager::Get()->RemoveListener(fastdelegate::MakeDelegate(this, &EntityManager::OnEntityMove), EventDataEntityMove::EventType);
    core::IEventManager::Get()->RemoveListener(fastdelegate::MakeDelegate(this, &EntityManager::OnEntityRelativeMove), EventDataEntityRelativeMove::EventType);
}

void wiicraft::EntityManager::Render(renderer::Renderer &renderer)
{
    for (const auto& entity : mPlayerEntities)
    {
        const math::Vector3f entityAABBPos = {static_cast<float>(entity.second->GetX() / 32.0f), static_cast<float>(entity.second->GetY() / 32.0f), static_cast<float>(entity.second->GetZ() / 32.0f)};
        renderer.DrawAABB({entityAABBPos + math::Vector3f(0.0f, .9f, 0.0f), math::Vector3f(0.3f, .9f, 0.3f)}, renderer::ColorRGBA::RED);
    }

    for (const auto& entity : mItemEntities)
    {
        const math::Vector3f entityAABBPos = {static_cast<float>(entity.second->GetX() / 32.0f), static_cast<float>(entity.second->GetY() / 32.0f), static_cast<float>(entity.second->GetZ() / 32.0f)};
        renderer.DrawAABB({entityAABBPos + math::Vector3f(0.0f, .2f, 0.0f), math::Vector3f(0.1f, .1f, 0.1f)}, renderer::ColorRGBA::RED);
    }
}

void wiicraft::EntityManager::OnPlayerEntitySpawn(core::IEventDataPtr eventData)
{
    std::shared_ptr<EventDataSpawnPlayerEntity> spawnEntity = std::static_pointer_cast<EventDataSpawnPlayerEntity>(eventData);
    mPlayerEntities[spawnEntity->GetEntity()->GetID()] = spawnEntity->GetEntity();
    PacketChatMessage msg("PlayerEntitySpawn");
    msg.Send();
}

void wiicraft::EntityManager::OnItemEntitySpawn(core::IEventDataPtr eventData)
{
    std::shared_ptr<EventDataSpawnItemEntity> itemEntity = std::static_pointer_cast<EventDataSpawnItemEntity>(eventData);
    mItemEntities[itemEntity->GetEntityID()] = std::make_shared<ItemEntity>(itemEntity->GetEntityID(), itemEntity->GetX(), itemEntity->GetY(),
                                                                      itemEntity->GetZ(), itemEntity->GetYaw(), itemEntity->GetPitch());
    PacketChatMessage msg("ItemEntitySpawn");
    msg.Send();

}

void wiicraft::EntityManager::OnEntityRemove(core::IEventDataPtr eventData)
{
    std::shared_ptr<EventDataDestroyEntity> entityData = std::static_pointer_cast<EventDataDestroyEntity>(eventData);
    auto playerFindIt = mPlayerEntities.find(entityData->GetEntityId());
    if (playerFindIt != mPlayerEntities.end())
    {
        mPlayerEntities.erase(playerFindIt);
        PacketChatMessage msg("EntityPlayerRemove");
        msg.Send();
        return;
    }
   auto itemFindIt = mItemEntities.find(entityData->GetEntityId());
   if (itemFindIt != mItemEntities.end())
   {
       mItemEntities.erase(itemFindIt);
       PacketChatMessage msg("EntityItemRemove");
       msg.Send();
       return;
   }
}

void wiicraft::EntityManager::OnEntityMove(core::IEventDataPtr eventData)
{
    std::shared_ptr<EventDataEntityMove> entityData = std::static_pointer_cast<EventDataEntityMove>(eventData);
    auto playerFindIt = mPlayerEntities.find(entityData->GetEntityID());
    if (playerFindIt != mPlayerEntities.end())
    {
        auto entity = playerFindIt->second;
        entity->SetPosition(entityData->GetX(), entityData->GetY(), entityData->GetZ());
        entity->SetYaw(entityData->GetYaw());
        entity->SetPitch(entityData->GetPitch());
        return;
    }
    auto itemFindIt = mItemEntities.find(entityData->GetEntityID());
    if (itemFindIt != mItemEntities.end())
    {
        auto entity = playerFindIt->second;
        entity->SetPosition(entityData->GetX(), entityData->GetY(), entityData->GetZ());
        entity->SetYaw(entityData->GetYaw());
        entity->SetPitch(entityData->GetPitch());
        return;
    }

}

void wiicraft::EntityManager::OnEntityRelativeMove(core::IEventDataPtr eventData)
{
    std::shared_ptr<EventDataEntityRelativeMove> entityData = std::static_pointer_cast<EventDataEntityRelativeMove>(eventData);
    auto playerFindIt = mPlayerEntities.find(entityData->GetEntityID());
    if (playerFindIt != mPlayerEntities.end())
    {
        playerFindIt->second->UpdatePositionAndRotation(entityData->GetX(), entityData->GetY(), entityData->GetZ(), entityData->GetYaw(), entityData->GetPitch());
        return;
    }
    auto itemFindIt = mItemEntities.find(entityData->GetEntityID());
    if (itemFindIt != mItemEntities.end())
    {
        itemFindIt->second->UpdatePositionAndRotation(entityData->GetX(), entityData->GetY(), entityData->GetZ(), entityData->GetYaw(), entityData->GetPitch());
        return;
    }
}


