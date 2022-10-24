#include "EventDataDestroyEntity.h"

constexpr core::EventType wiicraft::EventDataDestroyEntity::EventType;

wiicraft::EventDataDestroyEntity::EventDataDestroyEntity(int32_t entityId)
    : mEntityId(entityId)
{
}

const core::EventType& wiicraft::EventDataDestroyEntity::GetEventType() const
{
    return EventType;
}

core::IEventDataPtr wiicraft::EventDataDestroyEntity::Copy() const
{
    return std::make_shared<EventDataDestroyEntity>(mEntityId);
}

const char* wiicraft::EventDataDestroyEntity::GetName() const
{
    return "EventDataDestroyEntity";
}
