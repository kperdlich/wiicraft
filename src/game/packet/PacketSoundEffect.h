#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

class PacketSoundEffect : public Packet
{
public:
    PacketSoundEffect() : Packet(PACKET_SOUND_EFFECT) {}

    void Read(const net::Socket &socket) override
    {
        m_EID = socket.Read<int32_t>();
        m_X = socket.Read<int32_t>();
        m_Y = socket.Read<char>();
        m_Z = socket.Read<int32_t>();
        m_Data = socket.Read<int32_t>();

    }
    void Action() override
    {
    }
    Packet *CreateInstance() const override
    {
        return new PacketSoundEffect();
    }

protected:
    void SendContent(const net::Socket &socket) const override
    {
    }

    int32_t m_EID = 0;
    int32_t m_X = 0;
    char m_Y = 0;
    int32_t m_Z = 0;
    int32_t m_Data = 0;
};
