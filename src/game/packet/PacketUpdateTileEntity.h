#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

class PacketUpdateTileEntity : public Packet
{
public:
    PacketUpdateTileEntity() : Packet(PACKET_UPDATE_TILE_ENTITY) {}

    void Read(const net::Socket &socket) override
    {
        m_X = socket.Read<int32_t>();
        m_Y = socket.Read<int16_t>();
        m_Z = socket.Read<int32_t>();
        m_Action= socket.Read<char>();
        m_Custom1 = socket.Read<int32_t>();
        m_Custom2 = socket.Read<int32_t>();
        m_Custom3 = socket.Read<int32_t>();
    }
    void Action() override
    {
    }
    Packet *CreateInstance() const override
    {
        return new PacketUpdateTileEntity();
    }

protected:
    void SendContent(const net::Socket &socket) const override
    {
    }

    int32_t m_X = 0, m_Z = 0;
    int16_t m_Y = 0;
    char m_Action = 0;
    int32_t m_Custom1 = 0, m_Custom2 = 0, m_Custom3 = 0;
};
