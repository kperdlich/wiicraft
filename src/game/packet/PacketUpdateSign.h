#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

class PacketUpdateSign : public Packet
{
public:
    PacketUpdateSign() : Packet(PACKET_UPDATE_SIGN) {}

    void Read(const net::Socket &socket) override
    {
        m_X = socket.Read<int32_t>();
        m_Y = socket.Read<int16_t>();
        m_Z = socket.Read<int32_t>();
        m_Text1 = socket.ReadString();
        m_Text2 = socket.ReadString();
        m_Text3 = socket.ReadString();
        m_Text4 = socket.ReadString();
    }

    void Action() override
    {
    }
    Packet *CreateInstance() const override
    {
        return new PacketUpdateSign();
    }

protected:
    void SendContent(const net::Socket &socket) const override
    {
    }

    int32_t m_X = 0, m_Z = 0;
    int16_t m_Y = 0;
    std::string  m_Text1, m_Text2, m_Text3, m_Text4;
};
