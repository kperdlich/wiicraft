#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

class PacketSpawnPainting : public Packet
{
public:
    PacketSpawnPainting()
        : Packet(PACKET_SPAWN_PAINTING)
    {
    }

    void Read(const net::Socket& socket) override
    {
        m_EID = socket.Read<int32_t>();
        m_Title = socket.ReadString();
        m_X = socket.Read<int32_t>();
        m_Y = socket.Read<int32_t>();
        m_Z = socket.Read<int32_t>();
        m_Direction = socket.Read<int32_t>();
    }
    void Action() override
    {
    }
    Packet* CreateInstance() const override
    {
        return new PacketSpawnPainting();
    }

protected:
    void SendContent(const net::Socket& socket) const override
    {
    }

    int32_t m_EID = 0;
    std::string m_Title;
    int32_t m_X = 0, m_Y = 0, m_Z = 0, m_Direction = 0;
};
