#include "entity.h"

wiicraft::PlayerEntity::PlayerEntity(int32_t id, const std::string &name, int32_t x, int32_t y, int32_t z, int16_t currentItem, int8_t yaw, int8_t pitch)
    : mName(name),
      mID(id),
      mX(x),
      mY(y),
      mZ(z),
      mCurrentIten(currentItem),
      mPitch(pitch),
      mYaw(yaw)
{
}

wiicraft::ItemEntity::ItemEntity(int32_t id, int32_t x, int32_t y, int32_t z, int8_t yaw, int8_t pitch)
    : mID(id),
      mX(x),
      mY(y),
      mZ(z),
      mPitch(pitch),
      mYaw(yaw)
{
}
