#pragma once

#include "eventmanager.h"
#include "vector3f.h"

namespace wiicraft {

class EventDataSpawnPlayer : public core::BaseEventData
{
public:
    static constexpr core::EventType EventType = 0x931628de;

    EventDataSpawnPlayer(const math::Vector3f& spawnPosition);
    virtual const core::EventType& GetEventType() const override;
    virtual core::IEventDataPtr Copy() const override;
    virtual const char* GetName() const override;
    inline const math::Vector3f& GetSpawnPosition() const;

private:
    math::Vector3f mSpawnPosition;
};

inline const math::Vector3f& EventDataSpawnPlayer::GetSpawnPosition() const
{
    return mSpawnPosition;
}

}
