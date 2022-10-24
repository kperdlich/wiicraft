#pragma once

#include "Entity.h"
#include "EventManager.h"
#include "Renderer.h"
#include "Vector3f.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace wiicraft
{

    class EntityManager
    {
    public:
        EntityManager();
        ~EntityManager();
        EntityManager(const EntityManager&) = delete;
        EntityManager& operator=(const EntityManager&) = delete;
        EntityManager(EntityManager&&) = delete;
        EntityManager& operator=(EntityManager&&) = delete;

        void Render(renderer::Renderer& renderer);

    private:
        void OnPlayerEntitySpawn(core::IEventDataPtr eventData);
        void OnItemEntitySpawn(core::IEventDataPtr eventData);
        void OnEntityRemove(core::IEventDataPtr eventData);
        void OnEntityMove(core::IEventDataPtr eventData);
        void OnEntityRelativeMove(core::IEventDataPtr eventData);

    private:
        std::unordered_map<int32_t, std::shared_ptr<PlayerEntity>> mPlayerEntities;
        std::unordered_map<int32_t, std::shared_ptr<ItemEntity>> mItemEntities;
    };

} // namespace wiicraft
