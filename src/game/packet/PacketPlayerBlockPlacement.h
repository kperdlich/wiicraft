#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

// todo implement

class PacketPlayerBlockPlacement : public Packet
{
public:
    PacketPlayerBlockPlacement() : Packet(PACKET_PLAYER_BLOCK_PLACEMENT) {}

    void Read(const net::Socket &socket) override
    {

    }
    void Action() override
    {

    }
    Packet *CreateInstance() const override
    {
        return new PacketPlayerBlockPlacement();
    }

protected:
    void SendContent(const net::Socket &socket) const override
    {

    }
};
