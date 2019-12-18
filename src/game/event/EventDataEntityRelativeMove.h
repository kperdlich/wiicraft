#pragma once

#include <memory>
#include "eventmanager.h"

namespace wiicraft {

class EventDataEntityRelativeMove : public core::BaseEventData
{
public:
    static constexpr core::EventType EventType = 0x54267588;
    EventDataEntityRelativeMove(int32_t entityID, int8_t yaw, int8_t pitch, int8_t x, int8_t y, int8_t z);
    virtual const core::EventType& GetEventType() const override;
    virtual core::IEventDataPtr Copy() const override;
    virtual const char* GetName() const override;

    inline int32_t GetEntityID() const;
    inline int8_t GetX() const;
    inline int8_t GetY() const;
    inline int8_t GetZ() const;
    inline int8_t GetPitch() const;
    inline int8_t GetYaw() const;

private:
    int32_t mEntityID;
    int8_t mX, mY, mZ;
    int8_t mYaw, mPitch;

};

inline int32_t EventDataEntityRelativeMove::GetEntityID() const
{
    return mEntityID;
}

inline int8_t EventDataEntityRelativeMove::GetX() const
{
    return mX;
}

inline int8_t EventDataEntityRelativeMove::GetY() const
{
    return mY;
}

inline int8_t EventDataEntityRelativeMove::GetZ() const
{
    return mZ;
}

inline int8_t EventDataEntityRelativeMove::GetYaw() const
{
    return mYaw;
}

inline int8_t EventDataEntityRelativeMove::GetPitch() const
{
    return mPitch;
}

};

