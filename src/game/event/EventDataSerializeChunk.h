#pragma once

#include "eventmanager.h"
#include "compressed_chunk_data.h"

namespace wiicraft {

class EventDataSerializeChunk : public core::BaseEventData
{
public:
    static constexpr core::EventType EventType = 0xda4d2295;

    EventDataSerializeChunk(const wiicraft::CompressedChunkData& compressedChunkData);
    virtual const core::EventType& GetEventType() const override;
    virtual core::IEventDataPtr Copy() const override;
    virtual const char* GetName() const override;
    inline const wiicraft::CompressedChunkData& GetCompressedChunk() const;

private:
    wiicraft::CompressedChunkData mCompressedChunk;
};

inline const wiicraft::CompressedChunkData& EventDataSerializeChunk::GetCompressedChunk() const
{
    return mCompressedChunk;
}

}
