#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

class PacketDestroyEntity : public Packet
{
public:
    PacketDestroyEntity() : Packet(PACKET_DESTROY_ENTITY) {}

    void Read(const net::Socket &socket) override
    {
        m_EID = socket.Read<int32_t>();
    }
    void Action() override
    {
    }
    Packet *CreateInstance() const override
    {
        return new PacketDestroyEntity();
    }

protected:
    void SendContent(const net::Socket &socket) const override
    {
    }

    int32_t m_EID = 0;
};

