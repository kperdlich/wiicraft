#pragma once

#include "EventManager.h"
#include "Vector3f.h"

namespace wiicraft
{

    class EventDataSetPlayerPositionAndLook : public core::BaseEventData
    {
    public:
        static constexpr core::EventType EventType = 0xd8dedad5;

        EventDataSetPlayerPositionAndLook(const math::Vector3f& position, float yaw, float pitch, double stance, bool onGround);
        virtual const core::EventType& GetEventType() const override;
        virtual core::IEventDataPtr Copy() const override;
        virtual const char* GetName() const override;

        inline const math::Vector3f& GetPosition() const;
        inline float GetYaw() const;
        inline float GetPitch() const;
        inline double GetStance() const;
        inline bool GetOnGround() const;

    private:
        math::Vector3f mPosition;
        float mYaw, mPitch;
        double mStance;
        bool mOnGround;
    };

    inline const math::Vector3f& EventDataSetPlayerPositionAndLook::GetPosition() const
    {
        return mPosition;
    }

    inline float EventDataSetPlayerPositionAndLook::GetYaw() const
    {
        return mYaw;
    }

    inline float EventDataSetPlayerPositionAndLook::GetPitch() const
    {
        return mPitch;
    }

    inline double EventDataSetPlayerPositionAndLook::GetStance() const
    {
        return mStance;
    }

    inline bool EventDataSetPlayerPositionAndLook::GetOnGround() const
    {
        return mOnGround;
    }

} // namespace wiicraft
