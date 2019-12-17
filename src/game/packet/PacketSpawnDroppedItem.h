#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

class PacketSpawnDroppedItem : public Packet
{
public:
    PacketSpawnDroppedItem() : Packet(PACKET_SPAWN_DROPPED_ITEM) {}

    void Read(const net::Socket &socket) override
    {
        m_EID = socket.Read<int32_t>();
        m_Item = socket.Read<int16_t>();
        m_Count = socket.Read<char>();
        m_Data = socket.Read<int16_t>();
        m_X = socket.Read<int32_t>();
        m_Y = socket.Read<int32_t>();
        m_Z = socket.Read<int32_t>();
        m_Rotation = socket.Read<char>();
        m_Pitch = socket.Read<char>();
        m_Roll = socket.Read<char>();
    }

    void Action() override
    {

    }

    Packet *CreateInstance() const override
    {
        return new PacketSpawnDroppedItem();
    }

protected:
    void SendContent(const net::Socket &socket) const override {}

    int32_t m_EID = 0;
    int16_t m_Item = 0;
    char m_Count = 0;
    int16_t m_Data = 0;
    int32_t m_X = 0, m_Y = 0, m_Z = 0;
    char m_Rotation = 0;
    char m_Pitch = 0;
    char m_Roll = 0;

};

