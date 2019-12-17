#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

// todo implement

class PacketPlayer : public Packet
{
public:
    PacketPlayer(bool onGround = false) : Packet(PACKET_PLAYER), m_bOnGround(onGround) {}

    void Read(const net::Socket &socket) override
    {

    }
    void Action() override
    {

    }
    Packet *CreateInstance() const override
    {
       return new PacketPlayer();
    }

protected:
    void SendContent(const net::Socket &socket) const override
    {
        socket.Send<bool>(m_bOnGround);
    }

    bool m_bOnGround;
};

