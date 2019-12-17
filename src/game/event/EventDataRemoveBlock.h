#pragma once

#include "eventmanager.h"

namespace wiicraft {


class EventDataRemoveBlock : public core::BaseEventData
{
public:
    static constexpr core::EventType EventType = 0x5840b587;

    EventDataRemoveBlock(int32_t x, int8_t y, int32_t z, int8_t blockType);

    virtual const core::EventType& GetEventType() const override;
    virtual core::IEventDataPtr Copy() const override;
    virtual const char* GetName() const override;

    inline int32_t GetX() const;
    inline int32_t GetZ() const;
    inline int8_t GetBlockType() const;
    inline int8_t GetY() const;

private:
    int32_t mX;
    int32_t mZ;    
    int8_t mY;
    int8_t mBlockType;
};

inline int32_t EventDataRemoveBlock::GetX() const
{
    return mX;
}

inline int32_t EventDataRemoveBlock::GetZ() const
{
    return mZ;
}

inline int8_t EventDataRemoveBlock::GetBlockType() const
{
    return mBlockType;
}

inline int8_t EventDataRemoveBlock::GetY() const
{
    return mY;
}

}
