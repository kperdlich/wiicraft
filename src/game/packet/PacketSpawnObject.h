#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

class PacketSpawnObject : public Packet
{
public:
    PacketSpawnObject() : Packet(PACKET_SPAWN_OBJECT) {}

    void Read(const net::Socket &socket) override
    {
        ASSERT_TEXT(false, "PacketSpawnObject not implemented!");
        m_EID = socket.Read<int32_t>();
        m_Type = socket.Read<char>();
        m_X = socket.Read<int32_t>();
        m_Y = socket.Read<int32_t>();
        m_Z = socket.Read<int32_t>();
        m_AdditionalData = socket.Read<int32_t>();
    }
    void Action() override
    {

    }
    Packet *CreateInstance() const override
    {
        return new PacketSpawnObject();
    }

protected:
    void SendContent(const net::Socket &socket) const override
    {

    }

    int32_t m_EID = 0;
    char m_Type = 0;
    int32_t m_X = 0, m_Y = 0, m_Z = 0;
    int32_t m_AdditionalData = 0;
};
