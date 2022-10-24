#include "EventDataSendPlayerPosition.h"

constexpr core::EventType wiicraft::EventDataSendPlayerPosition::EventType;

const core::EventType& wiicraft::EventDataSendPlayerPosition::GetEventType() const
{
    return EventType;
}

core::IEventDataPtr wiicraft::EventDataSendPlayerPosition::Copy() const
{
    return std::make_shared<EventDataSendPlayerPosition>();
}

const char* wiicraft::EventDataSendPlayerPosition::GetName() const
{
    return "EventDataSendPlayerPosition";
}
