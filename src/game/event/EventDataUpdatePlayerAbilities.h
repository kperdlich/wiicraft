#pragma once

#include "EventManager.h"
#include <string>

namespace wiicraft
{

    class EventDataUpdatePlayerAbilities : public core::BaseEventData
    {
    public:
        static constexpr core::EventType EventType = 0x493a84e9;

        EventDataUpdatePlayerAbilities(bool invulnerability, bool isFlying, bool canFly, bool instandDestroy);
        virtual const core::EventType& GetEventType() const override;
        virtual core::IEventDataPtr Copy() const override;
        virtual const char* GetName() const override;

        inline bool GetInvulnerability() const;
        inline bool IsFlying() const;
        inline bool GetCanFlying() const;
        inline bool GetInstantDestroy() const;

    private:
        bool mInvulnerability;
        bool mIsFlying;
        bool mCanFly;
        bool mInstantDestroy;
    };

    inline bool EventDataUpdatePlayerAbilities::GetInvulnerability() const
    {
        return mInvulnerability;
    }

    inline bool EventDataUpdatePlayerAbilities::IsFlying() const
    {
        return mIsFlying;
    }

    inline bool EventDataUpdatePlayerAbilities::GetCanFlying() const
    {
        return mCanFly;
    }

    inline bool EventDataUpdatePlayerAbilities::GetInstantDestroy() const
    {
        return mInstantDestroy;
    }

} // namespace wiicraft
