#pragma once

#include "EntityManager.h"
#include "EventManager.h"
#include "Vector3f.h"
#include <memory>

namespace wiicraft
{

    class EventDataSpawnPlayerEntity : public core::BaseEventData
    {
    public:
        static constexpr core::EventType EventType = 0x5f39a9c4;
        EventDataSpawnPlayerEntity(std::shared_ptr<wiicraft::PlayerEntity> entity);
        virtual const core::EventType& GetEventType() const override;
        virtual core::IEventDataPtr Copy() const override;
        virtual const char* GetName() const override;

        inline std::shared_ptr<wiicraft::PlayerEntity> GetEntity();

    private:
        std::shared_ptr<wiicraft::PlayerEntity> mEntity;
    };

    inline std::shared_ptr<PlayerEntity> EventDataSpawnPlayerEntity::GetEntity()
    {
        return mEntity;
    }

}; // namespace wiicraft
