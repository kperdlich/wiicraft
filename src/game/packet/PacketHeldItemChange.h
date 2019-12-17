#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

class PacketHeldItemChange : public Packet
{
public:
    PacketHeldItemChange() : Packet(PACKET_HELD_ITEM_CHANGE) {}

    void Read(const net::Socket &socket) override
    {

    }
    void Action() override
    {

    }
    Packet *CreateInstance() const override
    {
        return new PacketHeldItemChange();
    }

protected:
    void SendContent(const net::Socket &socket) const override
    {

    }
};
