#include "EventDataUpdatePlayerHotbarData.h"

constexpr core::EventType wiicraft::EventDataUpdatePlayerHotbarData::EventType;

wiicraft::EventDataUpdatePlayerHotbarData::EventDataUpdatePlayerHotbarData(
    int8_t windowID, int16_t slot, const wiicraft::SlotData& slotData)
    : mSlotData(slotData)
    , mSlot(slot)
    , mWindowID(windowID)
{
}

const core::EventType& wiicraft::EventDataUpdatePlayerHotbarData::GetEventType() const
{
    return EventType;
}

core::IEventDataPtr wiicraft::EventDataUpdatePlayerHotbarData::Copy() const
{
    return std::make_shared<EventDataUpdatePlayerHotbarData>(mWindowID, mSlot, mSlotData);
}

const char* wiicraft::EventDataUpdatePlayerHotbarData::GetName() const
{
    return "EventDataUpdatePlayerHotbarData";
}
