#pragma once

#include "EventManager.h"
#include "SlotData.h"
#include <string>

namespace wiicraft
{

    class EventDataUpdatePlayerHotbarData : public core::BaseEventData
    {
    public:
        static constexpr core::EventType EventType = 0x4e054d40;

        EventDataUpdatePlayerHotbarData(int8_t windowID, int16_t slot, const SlotData& slotData);
        virtual const core::EventType& GetEventType() const override;
        virtual core::IEventDataPtr Copy() const override;
        virtual const char* GetName() const override;

        inline const SlotData& GetSlotData() const;
        inline int16_t GetSlot() const;
        inline int8_t GetWindowId() const;

    private:
        SlotData mSlotData;
        int16_t mSlot;
        int8_t mWindowID;
    };

    inline const SlotData& EventDataUpdatePlayerHotbarData::GetSlotData() const
    {
        return mSlotData;
    }

    inline int16_t EventDataUpdatePlayerHotbarData::GetSlot() const
    {
        return mSlot;
    }

    inline int8_t EventDataUpdatePlayerHotbarData::GetWindowId() const
    {
        return mWindowID;
    }

} // namespace wiicraft
