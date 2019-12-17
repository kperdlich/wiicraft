#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

class PacketEntityLookAndRelativeMove : public Packet
{
public:
    PacketEntityLookAndRelativeMove() : Packet(PACKET_ENTITY_LOOK_RELATIVE_MOVE) {}

    void Read(const net::Socket &socket) override
    {
        m_EID = socket.Read<int32_t>();
        m_DX = socket.Read<char>();
        m_DY = socket.Read<char>();
        m_DZ = socket.Read<char>();
        m_Yaw = socket.Read<char>();
        m_Pitch = socket.Read<char>();
    }

    void Action() override
    {
    }
    Packet *CreateInstance() const override
    {
        return new PacketEntityLookAndRelativeMove();
    }

protected:
    void SendContent(const net::Socket &socket) const override
    {
    }

    int32_t m_EID = 0;
    char m_Yaw = 0, m_Pitch = 0;
    char m_DX = 0, m_DY = 0, m_DZ = 0;
};
