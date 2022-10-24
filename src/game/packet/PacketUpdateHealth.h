#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

class PacketUpdateHealth : public Packet
{
public:
    PacketUpdateHealth()
        : Packet(PACKET_UPDATE_HEALTH)
    {
    }

    void Read(const net::Socket& socket) override
    {
        m_Health = socket.Read<int16_t>();
        m_Food = socket.Read<int16_t>();
        m_FoodSaturation = socket.Read<float>();
    }
    void Action() override
    {
    }
    Packet* CreateInstance() const override
    {
        return new PacketUpdateHealth();
    }

protected:
    int16_t m_Health = 0, m_Food = 0;
    float m_FoodSaturation = 0.0f;

    void SendContent(const net::Socket& socket) const override
    {
    }
};
