#include "EventDataServerConnected.h"

constexpr core::EventType wiicraft::EventDataServerConnected::EventType;

const core::EventType &wiicraft::EventDataServerConnected::GetEventType() const
{
    return EventType;
}

core::IEventDataPtr wiicraft::EventDataServerConnected::Copy() const
{
    return std::make_shared<EventDataServerConnected>();
}

const char *wiicraft::EventDataServerConnected::GetName() const
{
    return "EventDataServerConnected";
}
