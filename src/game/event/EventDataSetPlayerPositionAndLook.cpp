#include "EventDataSetPlayerPositionAndLook.h"

constexpr core::EventType wiicraft::EventDataSetPlayerPositionAndLook::EventType;

wiicraft::EventDataSetPlayerPositionAndLook::EventDataSetPlayerPositionAndLook(const math::Vector3f &position, float yaw,
                                                                               float pitch, double stance, bool onGround)
    : mPosition(position),
      mYaw(yaw),
      mPitch(pitch),
      mStance(stance),
      mOnGround(onGround)
{
}

const core::EventType &wiicraft::EventDataSetPlayerPositionAndLook::GetEventType() const
{
    return EventType;
}

core::IEventDataPtr wiicraft::EventDataSetPlayerPositionAndLook::Copy() const
{
    return std::make_shared<EventDataSetPlayerPositionAndLook>(mPosition, mYaw, mPitch, mStance, mOnGround);
}

const char *wiicraft::EventDataSetPlayerPositionAndLook::GetName() const
{
    return "EventDataSetPlayerPositionAndLook";
}
