#pragma once

#include "eventmanager.h"

namespace wiicraft {


class EventDataAllChunksInQueueSerialized : public core::BaseEventData
{
public:
    static constexpr core::EventType EventType = 0xa71c9351;

    virtual const core::EventType& GetEventType() const override;
    virtual core::IEventDataPtr Copy() const override;
    virtual const char* GetName() const override;
};
}

