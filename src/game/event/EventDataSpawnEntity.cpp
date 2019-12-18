#include "EventDataSpawnEntity.h"

constexpr core::EventType wiicraft::EventDataSpawnEntity::EventType;

wiicraft::EventDataSpawnEntity::EventDataSpawnEntity(std::shared_ptr<wiicraft::Entity> entity)
    : mEntity(entity)
{
}

const core::EventType &wiicraft::EventDataSpawnEntity::GetEventType() const
{
    return EventType;
}

core::IEventDataPtr wiicraft::EventDataSpawnEntity::Copy() const
{
    return std::make_shared<EventDataSpawnEntity>(std::make_shared<Entity>(*mEntity));
}

const char *wiicraft::EventDataSpawnEntity::GetName() const
{
    return "EventDataSpawnEntity";
}
