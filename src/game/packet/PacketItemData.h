#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

class PacketItemData : public Packet
{
public:
    PacketItemData() : Packet(PACKET_ITEM_DATA) {}

    ~PacketItemData()
    {
        if (m_Text)
            delete [] m_Text;
    }

    void Read(const net::Socket &socket) override
    {
        m_ItemType = socket.Read<int16_t>();
        m_ItemID = socket.Read<int16_t>();
        m_TextLength = socket.Read<unsigned char>();
        m_Text = new char[m_TextLength];
        socket.Read(m_Text, m_TextLength);
    }

    void Action() override
    {
    }
    Packet *CreateInstance() const override
    {
        return new PacketItemData();
    }

protected:
    void SendContent(const net::Socket &socket) const override
    {
    }

    int16_t m_ItemType = 0, m_ItemID = 0;
    unsigned char m_TextLength = 0;
    char* m_Text = nullptr;
};

