#pragma once

#include "EventManager.h"
#include <memory>

namespace wiicraft
{

    class EventDataDestroyEntity : public core::BaseEventData
    {
    public:
        static constexpr core::EventType EventType = 0x9edd8ad6;

        EventDataDestroyEntity(int32_t entityId);
        virtual const core::EventType& GetEventType() const override;
        virtual core::IEventDataPtr Copy() const override;
        virtual const char* GetName() const override;

        inline int32_t GetEntityId();

    private:
        int32_t mEntityId;
    };

    inline int32_t EventDataDestroyEntity::GetEntityId()
    {
        return mEntityId;
    }

}; // namespace wiicraft
