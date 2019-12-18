#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include "vector3f.h"
#include "renderer.h"
#include "eventmanager.h"

namespace wiicraft {

class Entity {
public:
    Entity(int32_t id, const std::string& name, int32_t x, int32_t y, int32_t z, int16_t currentItem, int8_t yaw, int8_t pitch);
    Entity(const Entity&) = default;
    Entity& operator = (const Entity&) = default;
    Entity(Entity&&) = default;
    Entity& operator = (Entity&&) = default;

    inline int32_t GetID() const;
    inline const std::string& GetName() const;
    inline int32_t GetX() const;
    inline int32_t GetY() const;
    inline int32_t GetZ() const;
    inline int16_t GetCurrentItem() const;
    inline int8_t GetYaw() const;
    inline int8_t GetPitch() const;

    inline void SetPosition(int32_t x, int32_t y, int32_t z);
    inline void SetYaw(int8_t yaw);
    inline void SetPitch(int8_t pitch);
    inline void UpdatePositionAndRotation(int32_t x, int32_t y, int32_t z, int8_t yaw, int8_t pitch);
private:
    std::string mName;
    int32_t mID, mX, mY, mZ; // Absolute Numbers - divide by 32 to get world pos
    int16_t mCurrentIten;
    int8_t mPitch, mYaw;
};

inline int32_t Entity::GetID() const
{
    return mID;
}

inline const std::string& Entity::GetName() const
{
    return mName;
}
inline int32_t Entity::GetX() const
{
    return mX;
}

inline int32_t Entity::GetY() const
{
    return mY;
}
inline int32_t Entity::GetZ() const
{
    return mZ;
}

inline int16_t Entity::GetCurrentItem() const
{
    return mCurrentIten;
}

inline int8_t Entity::GetYaw() const
{
    return mYaw;
}

inline int8_t Entity::GetPitch() const
{
    return mPitch;
}

inline void Entity::SetPosition(int32_t x, int32_t y, int32_t z)
{
    mX = x;
    mY = y;
    mZ = z;
}

inline void Entity::SetYaw(int8_t yaw)
{
    mYaw = yaw;
}

inline void Entity::SetPitch(int8_t pitch)
{
    mPitch = pitch;
}

inline void Entity::UpdatePositionAndRotation(int32_t x, int32_t y, int32_t z, int8_t yaw, int8_t pitch)
{
    mX += x;
    mY += y;
    mZ += z;
    //mYaw += yaw;
    //mPitch += pitch;
}

class EntityManager
{
public:
    EntityManager();
    ~EntityManager();
    EntityManager(const EntityManager&) = delete;
    EntityManager& operator = (const EntityManager&) = delete;
    EntityManager(EntityManager&&) = delete;
    EntityManager& operator = (EntityManager&&) = delete;

    void Render(renderer::Renderer& renderer);

private:
    void OnEntitySpawn(core::IEventDataPtr eventData);
    void OnEntityRemove(core::IEventDataPtr eventData);
    void OnEntityMove(core::IEventDataPtr eventData);
    void OnEntityRelativeMove(core::IEventDataPtr eventData);

private:
    std::unordered_map<int32_t, std::shared_ptr<Entity>> mEntities;
};

}

