#pragma once

#include "eventmanager.h"

namespace wiicraft {


class EventDataSendPlayerPosition : public core::BaseEventData
{
public:
    static constexpr core::EventType EventType = 0x4911d651;
    virtual const core::EventType& GetEventType() const override;
    virtual core::IEventDataPtr Copy() const override;
    virtual const char* GetName() const override;
};
}
