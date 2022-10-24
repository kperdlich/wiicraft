#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

class PacketTimeUpdate : public Packet
{
public:
    PacketTimeUpdate()
        : Packet(PACKET_TIME_UPDATE)
    {
    }

    void Read(const net::Socket& socket) override
    {
        m_time = socket.Read<int64_t>();
    }

    void Action() override
    {
        // todo implement
    }

    Packet* CreateInstance() const override
    {
        return new PacketTimeUpdate();
    }

protected:
    int64_t m_time = 0;
    void SendContent(const net::Socket& socket) const override
    {
    }
};
