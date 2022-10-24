#include "EventDataEntityRelativeMove.h"

constexpr core::EventType wiicraft::EventDataEntityRelativeMove::EventType;

wiicraft::EventDataEntityRelativeMove::EventDataEntityRelativeMove(int32_t entityID, int8_t yaw, int8_t pitch, int8_t x, int8_t y, int8_t z)
    : mEntityID(entityID)
    , mX(x)
    , mY(y)
    , mZ(z)
    , mYaw(yaw)
    , mPitch(pitch)
{
}

const core::EventType& wiicraft::EventDataEntityRelativeMove::GetEventType() const
{
    return EventType;
}

core::IEventDataPtr wiicraft::EventDataEntityRelativeMove::Copy() const
{
    return std::make_shared<EventDataEntityRelativeMove>(mEntityID, mYaw, mPitch, mX, mY, mZ);
}

const char* wiicraft::EventDataEntityRelativeMove::GetName() const
{
    return "EventDataEntityRelativeMove";
}
