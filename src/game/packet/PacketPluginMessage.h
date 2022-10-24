#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

class PacketPluginMessage : public Packet
{
public:
    PacketPluginMessage()
        : Packet(PACKET_PLUGIN_MESSAGE)
    {
    }

    ~PacketPluginMessage()
    {
        if (m_Data)
            delete[] m_Data;
    }

    void Read(const net::Socket& socket) override
    {
        m_Channel = socket.ReadString();
        m_Length = socket.Read<int16_t>();
        m_Data = new char[m_Length];
        socket.Read(m_Data, m_Length);
    }
    void Action() override
    {
    }
    Packet* CreateInstance() const override
    {
        return new PacketPluginMessage();
    }

protected:
    void SendContent(const net::Socket& socket) const override
    {
    }
    std::string m_Channel;
    char* m_Data = nullptr;
    int16_t m_Length = 0;
};
