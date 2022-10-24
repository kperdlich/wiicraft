#include "EventDataEntityMove.h"

constexpr core::EventType wiicraft::EventDataEntityMove::EventType;

wiicraft::EventDataEntityMove::EventDataEntityMove(int32_t entityID, int8_t yaw, int8_t pitch, int32_t x, int32_t y, int32_t z)
    : mEntityID(entityID)
    , mX(x)
    , mY(y)
    , mZ(z)
    , mYaw(yaw)
    , mPitch(pitch)
{
}

const core::EventType& wiicraft::EventDataEntityMove::GetEventType() const
{
    return EventType;
}

core::IEventDataPtr wiicraft::EventDataEntityMove::Copy() const
{
    return std::make_shared<EventDataEntityMove>(mEntityID, mYaw, mPitch, mX, mY, mZ);
}

const char* wiicraft::EventDataEntityMove::GetName() const
{
    return "EventDataEntityMove";
}
