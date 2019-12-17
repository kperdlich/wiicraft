#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

// todo implement

class PacketMultiBlockChange : public Packet
{
public:
    PacketMultiBlockChange() : Packet(PACKET_MULTI_BLOCK_CHANGE) {}

    ~PacketMultiBlockChange()
    {
        if (m_Data)
            delete [] m_Data;
    }

    void Read(const net::Socket &socket) override
    {
        m_ChunkX = socket.Read<int32_t>();
        m_ChunkZ = socket.Read<int32_t>();
        m_RecordCount = socket.Read<int16_t>();
        m_DataSize = socket.Read<int32_t>();
        m_Data = new unsigned char[m_DataSize];
        socket.Read(m_Data, m_DataSize);
    }

    void Action() override
    {
    }
    Packet *CreateInstance() const override
    {
        return new PacketMultiBlockChange();
    }

protected:
    void SendContent(const net::Socket &socket) const override
    {
    }

    int32_t m_ChunkX = 0, m_ChunkZ = 0;
    int16_t m_RecordCount = 0;
    int32_t m_DataSize = 0;
    unsigned char* m_Data = nullptr;
};
