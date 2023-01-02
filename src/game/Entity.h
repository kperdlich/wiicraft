#include <stdint.h>
#include <string>

namespace wiicraft
{

    class ItemEntity
    {
    public:
        ItemEntity(int32_t id, int32_t x, int32_t y, int32_t z, int8_t yaw, int8_t pitch);
        ItemEntity(const ItemEntity&) = default;
        ItemEntity& operator=(const ItemEntity&) = default;
        ItemEntity(ItemEntity&&) = default;
        ItemEntity& operator=(ItemEntity&&) = default;

        inline int32_t GetID() const;
        inline int32_t GetX() const;
        inline int32_t GetY() const;
        inline int32_t GetZ() const;
        inline int8_t GetYaw() const;
        inline int8_t GetPitch() const;

        inline void SetPosition(int32_t x, int32_t y, int32_t z);
        inline void SetYaw(int8_t yaw);
        inline void SetPitch(int8_t pitch);
        inline void UpdatePositionAndRotation(int32_t x, int32_t y, int32_t z, int8_t yaw, int8_t pitch);

    private:
        int16_t mItemId;
        int32_t mID, mX, mY, mZ; // Absolute Numbers - divide by 32 to get world pos
        int8_t mPitch, mYaw;
    };

    inline int32_t ItemEntity::GetID() const
    {
        return mID;
    }

    inline int32_t ItemEntity::GetX() const
    {
        return mX;
    }

    inline int32_t ItemEntity::GetY() const
    {
        return mY;
    }
    inline int32_t ItemEntity::GetZ() const
    {
        return mZ;
    }

    inline int8_t ItemEntity::GetYaw() const
    {
        return mYaw;
    }

    inline int8_t ItemEntity::GetPitch() const
    {
        return mPitch;
    }

    inline void ItemEntity::SetPosition(int32_t x, int32_t y, int32_t z)
    {
        mX = x;
        mY = y;
        mZ = z;
    }

    inline void ItemEntity::SetYaw(int8_t yaw)
    {
        mYaw = yaw;
    }

    inline void ItemEntity::SetPitch(int8_t pitch)
    {
        mPitch = pitch;
    }

    inline void ItemEntity::UpdatePositionAndRotation(int32_t x, int32_t y, int32_t z, int8_t yaw, int8_t pitch)
    {
        mX += x;
        mY += y;
        mZ += z;
        mYaw += yaw;
        mPitch += pitch;
    }

    class PlayerEntity
    {
    public:
        PlayerEntity(int32_t id, const std::string& name, int32_t x, int32_t y, int32_t z, int16_t currentItem, int8_t yaw, int8_t pitch);
        PlayerEntity(const PlayerEntity&) = default;
        PlayerEntity& operator=(const PlayerEntity&) = default;
        PlayerEntity(PlayerEntity&&) = default;
        PlayerEntity& operator=(PlayerEntity&&) = default;

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

    inline int32_t PlayerEntity::GetID() const
    {
        return mID;
    }

    inline const std::string& PlayerEntity::GetName() const
    {
        return mName;
    }
    inline int32_t PlayerEntity::GetX() const
    {
        return mX;
    }

    inline int32_t PlayerEntity::GetY() const
    {
        return mY;
    }
    inline int32_t PlayerEntity::GetZ() const
    {
        return mZ;
    }

    inline int16_t PlayerEntity::GetCurrentItem() const
    {
        return mCurrentIten;
    }

    inline int8_t PlayerEntity::GetYaw() const
    {
        return mYaw;
    }

    inline int8_t PlayerEntity::GetPitch() const
    {
        return mPitch;
    }

    inline void PlayerEntity::SetPosition(int32_t x, int32_t y, int32_t z)
    {
        mX = x;
        mY = y;
        mZ = z;
    }

    inline void PlayerEntity::SetYaw(int8_t yaw)
    {
        mYaw = yaw;
    }

    inline void PlayerEntity::SetPitch(int8_t pitch)
    {
        mPitch = pitch;
    }

    inline void PlayerEntity::UpdatePositionAndRotation(int32_t x, int32_t y, int32_t z, int8_t yaw, int8_t pitch)
    {
        mX += x;
        mY += y;
        mZ += z;
        mYaw += yaw;
        mPitch += pitch;
    }

} // namespace wiicraft
