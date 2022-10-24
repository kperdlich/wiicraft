#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

class PacketEnchantItem : public Packet
{
public:
    PacketEnchantItem()
        : Packet(PACKET_ENCHANT_ITEM)
    {
    }

    // Packet interface
public:
    void Read(const net::Socket& socket) override
    {
    }
    void Action() override
    {
    }
    Packet* CreateInstance() const override
    {
        return new PacketEnchantItem();
    }

protected:
    void SendContent(const net::Socket& socket) const override
    {
    }
};
