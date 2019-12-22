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
        renderer.DrawAABB({entityAABBPos + math::Vector3f(-0.6f, .9f, -0.6f), math::Vector3f(0.3f, .9f, 0.3f)}, renderer::ColorRGBA::RED);
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
    ASSERT(spawnEntity);
    mPlayerEntities[spawnEntity->GetEntity()->GetID()] = spawnEntity->GetEntity();    
}

void wiicraft::EntityManager::OnItemEntitySpawn(core::IEventDataPtr eventData)
{
    std::shared_ptr<EventDataSpawnItemEntity> itemEntity = std::static_pointer_cast<EventDataSpawnItemEntity>(eventData);
    ASSERT(itemEntity);
    mItemEntities[itemEntity->GetEntityID()] = std::make_shared<ItemEntity>(itemEntity->GetEntityID(), itemEntity->GetX(), itemEntity->GetY(),
                                                                      itemEntity->GetZ(), itemEntity->GetYaw(), itemEntity->GetPitch());
}

void wiicraft::EntityManager::OnEntityRemove(core::IEventDataPtr eventData)
{
    std::shared_ptr<EventDataDestroyEntity> eventRemoveData = std::static_pointer_cast<EventDataDestroyEntity>(eventData);
    ASSERT(eventRemoveData);
    auto playerFindIt = mPlayerEntities.find(eventRemoveData->GetEntityId());
    if (playerFindIt != mPlayerEntities.end())
    {
        mPlayerEntities.erase(playerFindIt);        
        return;
    }
   auto itemFindIt = mItemEntities.find(eventRemoveData->GetEntityId());
   if (itemFindIt != mItemEntities.end())
   {
       mItemEntities.erase(itemFindIt);       
       return;
   }
}

void wiicraft::EntityManager::OnEntityMove(core::IEventDataPtr eventData)
{
    std::shared_ptr<EventDataEntityMove> entityMoveData = std::static_pointer_cast<EventDataEntityMove>(eventData);
    ASSERT(entityMoveData);
    auto playerFindIt = mPlayerEntities.find(entityMoveData->GetEntityID());
    if (playerFindIt != mPlayerEntities.end())
    {
        auto entity = playerFindIt->second;
        entity->SetPosition(entityMoveData->GetX(), entityMoveData->GetY(), entityMoveData->GetZ());
        entity->SetYaw(entityMoveData->GetYaw());
        entity->SetPitch(entityMoveData->GetPitch());
        return;
    }
    auto itemFindIt = mItemEntities.find(entityMoveData->GetEntityID());
    if (itemFindIt != mItemEntities.end())
    {
        auto entity = itemFindIt->second;
        entity->SetPosition(entityMoveData->GetX(), entityMoveData->GetY(), entityMoveData->GetZ());
        entity->SetYaw(entityMoveData->GetYaw());
        entity->SetPitch(entityMoveData->GetPitch());
        return;
    }

}

void wiicraft::EntityManager::OnEntityRelativeMove(core::IEventDataPtr eventData)
{
    std::shared_ptr<EventDataEntityRelativeMove> entityRelativeMove = std::static_pointer_cast<EventDataEntityRelativeMove>(eventData);
    ASSERT(entityRelativeMove);
    auto playerFindIt = mPlayerEntities.find(entityRelativeMove->GetEntityID());
    if (playerFindIt != mPlayerEntities.end())
    {
        playerFindIt->second->UpdatePositionAndRotation(entityRelativeMove->GetX(), entityRelativeMove->GetY(), entityRelativeMove->GetZ(),
                                                        entityRelativeMove->GetYaw(), entityRelativeMove->GetPitch());
        return;
    }
    auto itemFindIt = mItemEntities.find(entityRelativeMove->GetEntityID());
    if (itemFindIt != mItemEntities.end())
    {
        itemFindIt->second->UpdatePositionAndRotation(entityRelativeMove->GetX(), entityRelativeMove->GetY(), entityRelativeMove->GetZ(),
                                                      entityRelativeMove->GetYaw(), entityRelativeMove->GetPitch());
        return;
    }
}


