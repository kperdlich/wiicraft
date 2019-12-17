#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

// todo implement

class PacketClickWindow : public Packet
{
public:
    PacketClickWindow() : Packet(PACKET_CLICK_WINDOW) {}

    void Read(const net::Socket &socket) override
    {

    }

    void Action() override
    {
    }
    Packet *CreateInstance() const override
    {
        return new PacketClickWindow();
    }

protected:
    void SendContent(const net::Socket &socket) const override
    {
    }

    char m_WindowID = 0;
    int16_t m_Slot = 0;
    char m_RightClick = 0;
    int16_t m_ActionNumber = 0;
    bool m_Shift = false;
    // todo add slot data

};

