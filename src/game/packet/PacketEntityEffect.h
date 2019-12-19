#pragma once

#include "Packet.h"
#include "PacketGlobals.h"
#include "core.h"

class PacketEntityEffect : public Packet
{
public:
    PacketEntityEffect() : Packet(PACKET_ENTITY_EFFECT) {}

    void Read(const net::Socket &socket) override
    {
        m_EID = socket.Read<int32_t>();
        m_EffectID = socket.Read<char>();
        m_Amplifier = socket.Read<char>();
        m_Duration = socket.Read<int16_t>();
        ASSERT(false);
    }
    void Action() override
    {
    }
    Packet *CreateInstance() const override
    {
        return new PacketEntityEffect();
    }

protected:
    void SendContent(const net::Socket &socket) const override
    {
    }

    int32_t m_EID = 0;
    char m_EffectID = 0, m_Amplifier = 0;
    int16_t m_Duration = 0;
};
