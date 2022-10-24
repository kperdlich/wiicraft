#pragma once

#include "CompressedChunkData.h"
#include "Core.h"
#include "EventDataSerializeChunk.h"
#include "EventManager.h"
#include "Packet.h"
#include "PacketGlobals.h"
#include <iostream>
#include <sstream>
#include <stdlib.h>

// todo implement

class PacketChunkData : public Packet
{
public:
    PacketChunkData()
        : Packet(PACKET_CHUNK_DATA)
    {
    }
    ~PacketChunkData()
    {
        // if (m_CompressedData)
        //     delete [] m_CompressedData;
        // m_CompressedData = nullptr;
    }

    void Read(const net::Socket& socket) override
    {
        m_chunkData.m_X = socket.Read<int32_t>();
        m_chunkData.m_Z = socket.Read<int32_t>();
        m_chunkData.m_bGroundUpCon = socket.Read<bool>();
        m_chunkData.m_PrimaryBitMap = socket.Read<uint16_t>();
        m_chunkData.m_AddBitMap = socket.Read<uint16_t>();
        m_chunkData.m_CompressedSize = socket.Read<int32_t>();
        socket.Read<int32_t>();
        m_chunkData.m_CompressedData = new unsigned char[m_chunkData.m_CompressedSize]; // this will be managed in the Serialize job!
        ASSERT(m_chunkData.m_CompressedData != nullptr);
        socket.Read(m_chunkData.m_CompressedData, m_chunkData.m_CompressedSize);
    }

    void Action() override
    {
        core::IEventManager::Get()->TriggerEvent(std::make_shared<wiicraft::EventDataSerializeChunk>(m_chunkData));
    }

    Packet* CreateInstance() const override
    {
        return new PacketChunkData();
    }

protected:
    void SendContent(const net::Socket& socket) const override
    {
    }

    wiicraft::CompressedChunkData m_chunkData;
};
