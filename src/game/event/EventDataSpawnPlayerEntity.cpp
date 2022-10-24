#include "EventDataSpawnPlayerEntity.h"

constexpr core::EventType wiicraft::EventDataSpawnPlayerEntity::EventType;

wiicraft::EventDataSpawnPlayerEntity::EventDataSpawnPlayerEntity(std::shared_ptr<wiicraft::PlayerEntity> entity)
    : mEntity(entity)
{
}

const core::EventType& wiicraft::EventDataSpawnPlayerEntity::GetEventType() const
{
    return EventType;
}

core::IEventDataPtr wiicraft::EventDataSpawnPlayerEntity::Copy() const
{
    return std::make_shared<EventDataSpawnPlayerEntity>(std::make_shared<PlayerEntity>(*mEntity));
}

const char* wiicraft::EventDataSpawnPlayerEntity::GetName() const
{
    return "EventDataSpawnEntity";
}
