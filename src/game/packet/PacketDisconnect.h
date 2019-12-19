#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

class PacketDisconnect : public Packet
{
public:
    PacketDisconnect() : Packet(PACKET_DISCONNECT) {}

    void Read(const net::Socket &socket) override
    {
        m_Reason = socket.ReadString();
    }

    void Action() override
    {        

    }

    Packet *CreateInstance() const override
    {
        return new PacketDisconnect();
    }

    std::string m_Reason;

protected:
    void SendContent(const net::Socket &socket) const override
    {
    }

};
