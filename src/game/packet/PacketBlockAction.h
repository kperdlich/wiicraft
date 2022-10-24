#pragma once

#include "Core.h"
#include "Packet.h"
#include "PacketGlobals.h"

class PacketBlockAction : public Packet
{
public:
    PacketBlockAction()
        : Packet(PACKET_BLOCK_ACTION)
    {
    }

    void Read(const net::Socket& socket) override
    {
        m_X = socket.Read<int32_t>();
        m_Y = socket.Read<int16_t>();
        m_Z = socket.Read<int32_t>();
        m_Byte_1 = socket.Read<char>();
        m_Byte_2 = socket.Read<char>();
    }

    void Action() override
    {
    }

    Packet* CreateInstance() const override
    {
        return new PacketBlockAction();
    }

protected:
    void SendContent(const net::Socket& socket) const override
    {
    }

    int32_t m_X = 0;
    int16_t m_Y = 0;
    int32_t m_Z = 0;
    char m_Byte_1 = 0, m_Byte_2 = 0;
};
