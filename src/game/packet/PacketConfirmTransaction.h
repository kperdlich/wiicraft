#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

class PacketConfirmTransaction : public Packet
{
public:
     PacketConfirmTransaction() : Packet(PACKET_CONFIRM_TRANSACTION) {}

     void Read(const net::Socket &socket) override
     {
         m_WindowID = socket.Read<char>();
         m_ActionNumber = socket.Read<uint16_t>();
         m_bAccepted = socket.Read<bool>();
     }
     void Action() override
     {
     }
     Packet *CreateInstance() const override
     {
         return new PacketConfirmTransaction();
     }

protected:
     void SendContent(const net::Socket &socket) const override
     {
     }

     char m_WindowID = 0;
     uint16_t m_ActionNumber = 0;
     bool m_bAccepted = false;
};

