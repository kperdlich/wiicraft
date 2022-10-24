#pragma once

#include "EventManager.h"
#include <string>

namespace wiicraft
{

    class EventDataUpdatePlayerListItem : public core::BaseEventData
    {
    public:
        static constexpr core::EventType EventType = 0xe631a804;

        EventDataUpdatePlayerListItem(std::string playerName, int16_t ping);
        virtual const core::EventType& GetEventType() const override;
        virtual core::IEventDataPtr Copy() const override;
        virtual const char* GetName() const override;

        inline const std::string& GetPlayerName() const;
        inline int16_t GetPing() const;

    private:
        std::string mPlayerName;
        int16_t mPing;
    };

    inline const std::string& EventDataUpdatePlayerListItem::GetPlayerName() const
    {
        return mPlayerName;
    }

    int16_t EventDataUpdatePlayerListItem::GetPing() const
    {
        return mPing;
    }

} // namespace wiicraft
