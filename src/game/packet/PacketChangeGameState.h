#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

class PacketChangeGameState : public Packet
{
public:
    PacketChangeGameState() : Packet(PACKET_CHANGE_GAME_STATE) {}

    void Read(const net::Socket &socket) override
    {
        m_Reason = socket.Read<char>();
        m_GameMode = socket.Read<char>();
    }

    void Action() override
    {
    }

    Packet *CreateInstance() const override
    {
        return new PacketChangeGameState();
    }

protected:
    void SendContent(const net::Socket &socket) const override
    {
    }

    char m_Reason = 0, m_GameMode = 0;

};

