#include "EventDataAllChunksInQueueSerialized.h"

constexpr core::EventType wiicraft::EventDataAllChunksInQueueSerialized::EventType;

const core::EventType &wiicraft::EventDataAllChunksInQueueSerialized::GetEventType() const
{
    return EventType;
}

core::IEventDataPtr wiicraft::EventDataAllChunksInQueueSerialized::Copy() const
{
    return std::make_shared<EventDataAllChunksInQueueSerialized>();
}

const char *wiicraft::EventDataAllChunksInQueueSerialized::GetName() const
{
    return "EventDataAllChunksInQueueSerialized";
}
