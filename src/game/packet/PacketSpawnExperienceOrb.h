#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

class PacketSpawnExperienceOrb : public Packet
{
public:
    PacketSpawnExperienceOrb()
        : Packet(PACKET_SPAWN_EXPERIENCE_ORB)
    {
    }

    void Read(const net::Socket& socket) override
    {
        m_EID = socket.Read<int32_t>();
        m_X = socket.Read<int32_t>();
        m_Y = socket.Read<int32_t>();
        m_Z = socket.Read<int32_t>();
        m_Count = socket.Read<int16_t>();
    }
    void Action() override
    {
    }

    Packet* CreateInstance() const override
    {
        return new PacketSpawnExperienceOrb();
    }

protected:
    void SendContent(const net::Socket& socket) const override
    {
    }

    int32_t m_EID = 0, m_X = 0, m_Y = 0, m_Z = 0;
    int16_t m_Count = 0;
};
