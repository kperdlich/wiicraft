#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

class PacketChatMessage : public Packet
{
public:
     PacketChatMessage() : Packet(PACKET_CHAT_MESSAGE) {}

     void Read(const net::Socket &socket) override
     {
        m_chatMessage = socket.ReadString();
     }

     void Action() override
     {
        // todo implement
     }

     Packet *CreateInstance() const override
     {
        return new PacketChatMessage();
     }

protected:
     std::string m_chatMessage;
     void SendContent(const net::Socket &socket) const override
     {
         // todo implement
     }
};
