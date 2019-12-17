#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

class PacketThunderbolt : public Packet
{
public:
    PacketThunderbolt() : Packet(PACKET_THUNDERBOLT) {}

    void Read(const net::Socket &socket) override
    {
        m_EID = socket.Read<int32_t>();
        m_Unkown = socket.Read<char>();
        m_X = socket.Read<int32_t>();
        m_Y = socket.Read<int32_t>();
        m_Z = socket.Read<int32_t>();
    }
    void Action() override
    {
    }
    Packet *CreateInstance() const override
    {
        return new PacketThunderbolt();
    }

protected:
    void SendContent(const net::Socket &socket) const override
    {
    }

    int32_t m_EID = 0;
    char m_Unkown = 0;
    int32_t m_X = 0, m_Y = 0, m_Z = 0;
};
