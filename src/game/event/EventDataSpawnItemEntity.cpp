#include "EventDataSpawnItemEntity.h"

constexpr core::EventType wiicraft::EventDataSpawnItemEntity::EventType;

wiicraft::EventDataSpawnItemEntity::EventDataSpawnItemEntity(int32_t entityId, int32_t x, int32_t y, int32_t z, int8_t yaw, int8_t pitch)
    : mEntityID(entityId)
    , mX(x)
    , mY(y)
    , mZ(z)
    , mPitch(pitch)
    , mYaw(yaw)
{
}

const core::EventType& wiicraft::EventDataSpawnItemEntity::GetEventType() const
{
    return EventType;
}

core::IEventDataPtr wiicraft::EventDataSpawnItemEntity::Copy() const
{
    return std::make_shared<EventDataSpawnItemEntity>(mEntityID, mX, mY, mZ, mYaw, mPitch);
}

const char* wiicraft::EventDataSpawnItemEntity::GetName() const
{
    return "EventDataSpawnItemEntity";
}
