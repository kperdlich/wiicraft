#include "EventDataUpdatePlayerListItem.h"

constexpr core::EventType wiicraft::EventDataUpdatePlayerListItem::EventType;

wiicraft::EventDataUpdatePlayerListItem::EventDataUpdatePlayerListItem(std::string playerName, int16_t ping)
    : mPlayerName(playerName)
    , mPing(ping)
{
}

const core::EventType& wiicraft::EventDataUpdatePlayerListItem::GetEventType() const
{
    return EventType;
}

core::IEventDataPtr wiicraft::EventDataUpdatePlayerListItem::Copy() const
{
    return std::make_shared<EventDataUpdatePlayerListItem>(mPlayerName, mPing);
}

const char* wiicraft::EventDataUpdatePlayerListItem::GetName() const
{
    return "EventDataUpdatePlayerListItem";
}
