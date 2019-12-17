#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

// todo implement

class PacketServerListPing : public Packet
{
public:
    PacketServerListPing() : Packet(PACKET_SERVER_LIST_PING) {}

    void Read(const net::Socket &socket) override
    {
    }
    void Action() override
    {
    }
    Packet *CreateInstance() const override
    {
        return new PacketServerListPing();
    }

protected:
    void SendContent(const net::Socket &socket) const override
    {
    }
};

