#pragma once

#include "Packet.h"
#include "PacketGlobals.h"
#include "core.h"

class PacketEntityVelocity : public Packet
{
public:
    PacketEntityVelocity() : Packet(PACKET_ENTITY_VELOCITY) {}

    void Read(const net::Socket &socket) override
    {
        m_EID = socket.Read<int32_t>();
        m_VelocityX = socket.Read<int16_t>();
        m_VelocityY = socket.Read<int16_t>();
        m_VelocityZ = socket.Read<int16_t>();
    }
    void Action() override
    {
    }
    Packet *CreateInstance() const override
    {
        return new PacketEntityVelocity();
    }

protected:
    void SendContent(const net::Socket &socket) const override
    {
    }

    int32_t m_EID = 0;
    int16_t m_VelocityX = 0, m_VelocityY = 0, m_VelocityZ = 0;
};
