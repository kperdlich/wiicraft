#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

class PacketEntityEquipment : public Packet
{
public:
    PacketEntityEquipment()
        : Packet(PACKET_ENTITY_EQUIPMENT)
    {
    }

    void Read(const net::Socket& socket) override
    {
        m_EID = socket.Read<int32_t>();
        m_Slot = socket.Read<int16_t>();
        m_ItemID = socket.Read<int16_t>();
        m_Damage = socket.Read<int16_t>();
    }
    void Action() override
    {
    }
    Packet* CreateInstance() const override
    {
        return new PacketEntityEquipment();
    }

protected:
    int32_t m_EID = 0;
    int16_t m_Slot = 0, m_ItemID = 0, m_Damage = 0;

    void SendContent(const net::Socket& socket) const override
    {
    }
};
