#include "EventDataSpawnPlayer.h"

constexpr core::EventType wiicraft::EventDataSpawnPlayer::EventType;

wiicraft::EventDataSpawnPlayer::EventDataSpawnPlayer(const math::Vector3f &spawnPosition)
    : mSpawnPosition(spawnPosition) {}

const core::EventType &wiicraft::EventDataSpawnPlayer::GetEventType() const
{
    return EventType;
}

core::IEventDataPtr wiicraft::EventDataSpawnPlayer::Copy() const
{
    return std::make_shared<EventDataSpawnPlayer>(mSpawnPosition);
}

const char *wiicraft::EventDataSpawnPlayer::GetName() const
{
    return "EventDataSpawnPlayer";
}
