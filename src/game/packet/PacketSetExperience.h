#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

class PacketSetExperience : public Packet
{
public:
    PacketSetExperience()
        : Packet(PACKET_SET_EXPERIENCE)
    {
    }

    void Read(const net::Socket& socket) override
    {
        m_ExperienceBar = socket.Read<float>();
        m_Level = socket.Read<uint16_t>();
        m_TotalExperience = socket.Read<uint16_t>();
    }
    void Action() override
    {
    }
    Packet* CreateInstance() const override
    {
        return new PacketSetExperience();
    }

protected:
    void SendContent(const net::Socket& socket) const override
    {
    }

    float m_ExperienceBar = 0.0f;
    uint16_t m_Level = 0, m_TotalExperience = 0;
};
