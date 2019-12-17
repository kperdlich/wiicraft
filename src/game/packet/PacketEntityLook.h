#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

class PacketEntityLook : public Packet
{
public:
    PacketEntityLook() : Packet(PACKET_ENTITY_LOOK) {}

    void Read(const net::Socket &socket) override
    {
        m_EID = socket.Read<int32_t>();
        m_Yaw = socket.Read<char>();
        m_Pitch = socket.Read<char>();
    }

    void Action() override
    {
    }

    Packet* CreateInstance() const override
    {
        return new PacketEntityLook();
    }

protected:
    void SendContent(const net::Socket &socket) const override
    {
    }

    int32_t m_EID = 0;
    char m_Yaw = 0, m_Pitch = 0;

};

