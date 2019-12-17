#include "EventDataSerializeChunk.h"

constexpr core::EventType wiicraft::EventDataSerializeChunk::EventType;

wiicraft::EventDataSerializeChunk::EventDataSerializeChunk(const wiicraft::CompressedChunkData& compressedChunkData)
    : mCompressedChunk(compressedChunkData)
{
}

const core::EventType &wiicraft::EventDataSerializeChunk::GetEventType() const
{
    return EventType;
}

core::IEventDataPtr wiicraft::EventDataSerializeChunk::Copy() const
{
    return std::make_shared<EventDataSerializeChunk>(mCompressedChunk);
}

const char *wiicraft::EventDataSerializeChunk::GetName() const
{
    return "EventDataSerializeChunk";
}
