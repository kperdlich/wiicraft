#pragma once

#include "EventManager.h"

namespace wiicraft
{

    class EventDataServerConnected : public core::BaseEventData
    {
    public:
        static constexpr core::EventType EventType = 0x2570fe3e;

        virtual const core::EventType& GetEventType() const override;
        virtual core::IEventDataPtr Copy() const override;
        virtual const char* GetName() const override;
    };
} // namespace wiicraft
