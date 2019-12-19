#pragma once

#include "Packet.h"
#include "PacketGlobals.h"
#include "core.h"

class PacketUpdateWindowProperty : public Packet
{
public:
    PacketUpdateWindowProperty() : Packet(PACKET_UPDATE_WINDOW_PROPERTY) {}

    void Read(const net::Socket &socket) override
    {
        ASSERT(false);
        m_WindowID = socket.Read<char>();
        m_Property = socket.Read<int16_t>();
        m_Value = socket.Read<int16_t>();
    }
    void Action() override
    {
    }
    Packet *CreateInstance() const override
    {
        return new PacketUpdateWindowProperty();
    }

protected:
    void SendContent(const net::Socket &socket) const override
    {
    }

    char m_WindowID = 0;
    int16_t m_Property = 0, m_Value = 0;

};
