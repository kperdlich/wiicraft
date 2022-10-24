#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

class PacketRemoveEntityEffect : public Packet
{
public:
    PacketRemoveEntityEffect()
        : Packet(PACKET_REMOVE_ENTITY_EFFECT)
    {
    }

    void Read(const net::Socket& socket) override
    {
        m_EID = socket.Read<int32_t>();
        m_EffectID = socket.Read<char>();
    }

    void Action() override
    {
    }

    Packet* CreateInstance() const override
    {
        return new PacketRemoveEntityEffect();
    }

protected:
    void SendContent(const net::Socket& socket) const override
    {
    }

    int32_t m_EID = 0;
    char m_EffectID = 0;
};
