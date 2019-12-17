#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

// todo implement

class PacketPlayerPosition : public Packet
{
public:

   PacketPlayerPosition(double x, double y, double z, double stance, bool onGround) :
       Packet(PACKET_PLAYER_POSITION), m_X(x), m_Y(y), m_Z(z), m_Stance(stance), m_bOnGround(onGround) {}

   PacketPlayerPosition() : Packet(PACKET_PLAYER_POSITION) {}

   void Read(const net::Socket &socket) override
   {

   }
   void Action() override
   {

   }
   Packet *CreateInstance() const override
   {
        return new PacketPlayerPosition();
   }

protected:
   void SendContent(const net::Socket &socket) const override
   {
        socket.Send<double>(m_X);
        socket.Send<double>(m_Y);
        socket.Send<double>(m_Stance);
        socket.Send<double>(m_Z);
        socket.Send<bool>(m_bOnGround);
   }

   double m_X, m_Y, m_Z, m_Stance;
   bool m_bOnGround = false;
};
