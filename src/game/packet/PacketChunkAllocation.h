#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

class PacketChunkAllocation : public Packet
{
public:
    PacketChunkAllocation()
        : Packet(PACKET_CHUNK_ALLOCATION)
    {
    }

    void Read(const net::Socket& socket) override
    {
        m_x = socket.Read<int32_t>();
        m_y = socket.Read<int32_t>();
        m_bMode = socket.Read<bool>();
    }

    void Action() override
    {
    }

    Packet* CreateInstance() const override
    {
        return new PacketChunkAllocation();
    }

protected:
    void SendContent(const net::Socket& socket) const override
    {
    }
    int32_t m_x = 0, m_y = 0;
    bool m_bMode = false;
};
