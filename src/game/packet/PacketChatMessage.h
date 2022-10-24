#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

class PacketChatMessage : public Packet
{
public:
    PacketChatMessage()
        : Packet(PACKET_CHAT_MESSAGE)
    {
    }
    PacketChatMessage(const std::string& chatMessage)
        : Packet(PACKET_CHAT_MESSAGE)
        , m_chatMessage(chatMessage)
    {
    }

    void Read(const net::Socket& socket) override
    {
        m_chatMessage = socket.ReadString();
    }

    void Action() override
    {
        // todo implement
    }

    Packet* CreateInstance() const override
    {
        return new PacketChatMessage();
    }

protected:
    void SendContent(const net::Socket& socket) const override
    {
        socket.Send<int16_t>(m_chatMessage.length());
        socket.SendStringAsUtf16(m_chatMessage);
    }

    std::string m_chatMessage;
};
