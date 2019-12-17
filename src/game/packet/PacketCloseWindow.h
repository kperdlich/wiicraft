#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

class PacketCloseWindow : public Packet
{
public:
    PacketCloseWindow() : Packet(PACKET_CLOSE_WINDOW) {}

    void Read(const net::Socket &socket) override
    {
        m_WindowID = socket.Read<char>();
    }

    void Action() override
    {
    }

    Packet *CreateInstance() const override
    {
        return new PacketCloseWindow();
    }

protected:
    void SendContent(const net::Socket &socket) const override
    {
    }

    char m_WindowID = 0;
};

