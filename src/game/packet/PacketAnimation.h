#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

class PacketAnimation : public Packet
{
public:
    PacketAnimation()
        : Packet(PACKET_ANIMATION)
    {
    }

    void Read(const net::Socket& socket) override
    {
        m_EID = socket.Read<int32_t>();
        m_Animation = socket.Read<char>();
    }
    void Action() override
    {
    }

    Packet* CreateInstance() const override
    {
        return new PacketAnimation();
    }

protected:
    void SendContent(const net::Socket& socket) const override
    {
    }

    int32_t m_EID = 0;
    char m_Animation = 0;
};
