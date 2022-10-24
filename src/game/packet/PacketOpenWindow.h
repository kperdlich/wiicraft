#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

class PacketOpenWindow : public Packet
{
public:
    PacketOpenWindow()
        : Packet(PACKET_OPEN_WINDOW)
    {
    }

    void Read(const net::Socket& socket) override
    {
        m_WindowID = socket.Read<char>();
        m_InventoryType = socket.Read<char>();
        m_WindowTitle = socket.ReadString();
        m_NumberOfSlots = socket.Read<char>();
    }

    void Action() override
    {
    }

    Packet* CreateInstance() const override
    {
        return new PacketOpenWindow();
    }

protected:
    void SendContent(const net::Socket& socket) const override
    {
    }

    char m_WindowID = 0, m_InventoryType = 0;
    std::string m_WindowTitle;
    char m_NumberOfSlots = 0;
};
