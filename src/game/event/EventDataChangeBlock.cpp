#include "EventDataChangeBlock.h"

constexpr core::EventType wiicraft::EventDataChangeBlock::EventType;

wiicraft::EventDataChangeBlock::EventDataChangeBlock(int32_t x, int8_t y, int32_t z, int8_t blockType)
    : mX(x)
    , mZ(z)
    , mY(y)
    , mBlockType(blockType)
{
}

const core::EventType& wiicraft::EventDataChangeBlock::GetEventType() const
{
    return EventType;
}

core::IEventDataPtr wiicraft::EventDataChangeBlock::Copy() const
{
    return std::make_shared<EventDataChangeBlock>(mX, mY, mZ, mBlockType);
}

const char* wiicraft::EventDataChangeBlock::GetName() const
{
    return "EventDataRemoveBlock";
}
