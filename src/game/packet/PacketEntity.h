#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

class PacketEntity : public Packet
{
public:
    PacketEntity()
        : Packet(PACKET_ENTITY)
    {
    }

    void Read(const net::Socket& socket) override
    {
        m_EID = socket.Read<int32_t>();
    }
    void Action() override
    {
    }
    Packet* CreateInstance() const override
    {
        return new PacketEntity();
    }

protected:
    void SendContent(const net::Socket& socket) const override
    {
    }
    int32_t m_EID = 0;
};
