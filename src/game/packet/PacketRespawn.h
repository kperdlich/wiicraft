#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

class PacketRespawn : public Packet
{
public:
    PacketRespawn()
        : Packet(PACKET_RESPAWN)
    {
    }

    void Read(const net::Socket& socket) override
    {
        m_Dimension = socket.Read<int32_t>();
        m_Difficulty = socket.Read<char>();
        m_CreativeMode = socket.Read<char>();
        m_WorldHeight = socket.Read<int16_t>();
        m_LevelType = socket.ReadString();
    }

    void Action() override
    {
    }

    Packet* CreateInstance() const override
    {
        return new PacketRespawn();
    }

protected:
    void SendContent(const net::Socket& socket) const override
    {
    }

    int32_t m_Dimension = 0;
    char m_Difficulty = 0, m_CreativeMode = 0;
    int16_t m_WorldHeight = 0;
    std::string m_LevelType;
};
