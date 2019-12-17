#include "EventDataRemoveBlock.h"

constexpr core::EventType wiicraft::EventDataRemoveBlock::EventType;

wiicraft::EventDataRemoveBlock::EventDataRemoveBlock(int32_t x, int8_t y, int32_t z, int8_t blockType)
    : mX(x),
      mZ(z),     
      mY(y),
      mBlockType(blockType)
{}

const core::EventType &wiicraft::EventDataRemoveBlock::GetEventType() const
{
    return EventType;
}

core::IEventDataPtr wiicraft::EventDataRemoveBlock::Copy() const
{
    return std::make_shared<EventDataRemoveBlock>(mX, mY, mZ, mBlockType);
}

const char *wiicraft::EventDataRemoveBlock::GetName() const
{
    return "EventDataRemoveBlock";
}
