#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

// todo implement

class PacketEntityAction : public Packet
{
public:
    PacketEntityAction() : Packet(PACKET_ENTITY_ACTION) {}

    void Read(const net::Socket &socket) override
    {

    }
    void Action() override
    {

    }
    Packet *CreateInstance() const override
    {
        return new PacketEntityAction();
    }

protected:
    void SendContent(const net::Socket &socket) const override
    {

    }
};
