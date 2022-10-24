#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

class PacketUseBed : public Packet
{
public:
    PacketUseBed()
        : Packet(PACKET_USE_BED)
    {
    }

    void Read(const net::Socket& socket) override
    {
        m_EID = socket.Read<int32_t>();
        m_Unkown = socket.Read<char>();
        m_X = socket.Read<int32_t>();
        m_Y = socket.Read<char>();
        m_Z = socket.Read<int32_t>();
    }
    void Action() override
    {
    }
    Packet* CreateInstance() const override
    {
        return new PacketUseBed();
    }

protected:
    void SendContent(const net::Socket& socket) const override
    {
    }

    int32_t m_EID = 0;
    char m_Unkown = 0;
    int32_t m_X = 0;
    char m_Y = 0;
    int32_t m_Z = 0;
};
