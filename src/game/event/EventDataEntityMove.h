#pragma once

#include "EventManager.h"
#include <memory>

namespace wiicraft
{

    class EventDataEntityMove : public core::BaseEventData
    {
    public:
        static constexpr core::EventType EventType = 0xe5253d6c;

        EventDataEntityMove(int32_t entityID, int8_t yaw, int8_t pitch, int32_t x, int32_t y, int32_t z);
        virtual const core::EventType& GetEventType() const override;
        virtual core::IEventDataPtr Copy() const override;
        virtual const char* GetName() const override;

        inline int32_t GetEntityID() const;
        inline int32_t GetX() const;
        inline int32_t GetY() const;
        inline int32_t GetZ() const;
        inline int8_t GetPitch() const;
        inline int8_t GetYaw() const;

    private:
        int32_t mEntityID;
        int32_t mX, mY, mZ;
        int8_t mYaw, mPitch;
    };

    inline int32_t EventDataEntityMove::GetEntityID() const
    {
        return mEntityID;
    }

    inline int32_t EventDataEntityMove::GetX() const
    {
        return mX;
    }

    inline int32_t EventDataEntityMove::GetY() const
    {
        return mY;
    }

    inline int32_t EventDataEntityMove::GetZ() const
    {
        return mZ;
    }

    inline int8_t EventDataEntityMove::GetYaw() const
    {
        return mYaw;
    }

    inline int8_t EventDataEntityMove::GetPitch() const
    {
        return mPitch;
    }

}; // namespace wiicraft
