#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

class PacketEntityTeleport : public Packet
{
public:
    PacketEntityTeleport() : Packet(PACKET_ENTITY_TELEPORT) {}

    void Read(const net::Socket &socket) override
    {
        m_EID = socket.Read<int32_t>();
        m_X = socket.Read<int32_t>();
        m_Y = socket.Read<int32_t>();
        m_Z = socket.Read<int32_t>();
        m_Yaw = socket.Read<char>();
        m_Pitch = socket.Read<char>();
    }

    void Action() override
    {
    }

    Packet *CreateInstance() const override
    {
        return new PacketEntityTeleport();
    }

protected:
    void SendContent(const net::Socket &socket) const override
    {
    }

    int32_t m_EID = 0;
    int32_t m_X = 0, m_Y = 0, m_Z = 0;
    char m_Yaw = 0, m_Pitch = 0;
};
