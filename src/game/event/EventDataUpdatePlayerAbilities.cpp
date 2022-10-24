#include "EventDataUpdatePlayerAbilities.h"

constexpr core::EventType wiicraft::EventDataUpdatePlayerAbilities::EventType;

wiicraft::EventDataUpdatePlayerAbilities::EventDataUpdatePlayerAbilities(
    bool invulnerability, bool isFlying, bool canFly, bool instandDestroy)
    : mInvulnerability(invulnerability)
    , mIsFlying(isFlying)
    , mCanFly(canFly)
    , mInstantDestroy(instandDestroy)
{
}

const core::EventType& wiicraft::EventDataUpdatePlayerAbilities::GetEventType() const
{
    return EventType;
}

core::IEventDataPtr wiicraft::EventDataUpdatePlayerAbilities::Copy() const
{
    return std::make_shared<EventDataUpdatePlayerAbilities>(mInvulnerability, mIsFlying, mCanFly, mInstantDestroy);
}

const char* wiicraft::EventDataUpdatePlayerAbilities::GetName() const
{
    return "EventDataUpdatePlayerAbilities";
}
