#pragma once

#include <memory>
#include "eventmanager.h"
#include "vector3f.h"
#include "EntityManager.h"

namespace wiicraft {

class EventDataSpawnItemEntity : public core::BaseEventData
{
public:
    static constexpr core::EventType EventType = 0xc9cfcfd3;

    EventDataSpawnItemEntity(int32_t entityId, int32_t x, int32_t y, int32_t z, int8_t yaw, int8_t pitch);
    virtual const core::EventType& GetEventType() const override;
    virtual core::IEventDataPtr Copy() const override;
    virtual const char* GetName() const override;

    inline int32_t GetEntityID() const;
    inline int32_t GetX() const;
    inline int32_t GetY() const;
    inline int32_t GetZ() const;
    inline int8_t GetYaw() const;
    inline int8_t GetPitch() const;

private:
    int32_t mEntityID, mX, mY, mZ;
    int8_t mPitch, mYaw;
};

inline int32_t EventDataSpawnItemEntity::GetEntityID() const
{
    return mEntityID;
}

inline int32_t EventDataSpawnItemEntity::GetX() const
{
    return mX;
}

inline int32_t EventDataSpawnItemEntity::GetY() const
{
    return mY;
}
inline int32_t EventDataSpawnItemEntity::GetZ() const
{
    return mZ;
}

inline int8_t EventDataSpawnItemEntity::GetYaw() const
{
    return mYaw;
}

inline int8_t EventDataSpawnItemEntity::GetPitch() const
{
    return mPitch;
}

};

