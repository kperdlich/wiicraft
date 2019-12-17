#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

class PacketCreativeInventoryAction : public Packet
{
public:
    PacketCreativeInventoryAction() : Packet(PACKET_CREATIVE_INVENTORY_ACTION) {}

    void Read(const net::Socket &socket) override
    {
        m_Slot = socket.Read<int16_t>();
        ReadSlotData(m_SlotData, socket);
    }
    void Action() override
    {
    }
    Packet *CreateInstance() const override
    {
        return new PacketCreativeInventoryAction();
    }

protected:
    void SendContent(const net::Socket &socket) const override
    {
    }

    int16_t m_Slot = 0;
    SlotData m_SlotData;
};


