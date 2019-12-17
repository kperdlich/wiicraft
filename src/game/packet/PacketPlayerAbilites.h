#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

class PacketPlayerAbilites : public Packet
{
public:
    PacketPlayerAbilites() : Packet(PACKET_PLAYER_ABILITIES) {}

    void Read(const net::Socket &socket) override
    {
        m_bInvulnerability = socket.Read<bool>();
        m_bIsflying = socket.Read<bool>();
        m_bCanfly = socket.Read<bool>();
        m_bInstantDestroy = socket.Read<bool>();
    }

    void Action() override
    {
        // todo implement
    }

    Packet *CreateInstance() const override
    {
        return new PacketPlayerAbilites();
    }

protected:
    void SendContent(const net::Socket &socket) const override
    {
        // todo implement
    }

    bool m_bInvulnerability = false,
    m_bIsflying = false,
    m_bCanfly = false,
    m_bInstantDestroy = false;
};
