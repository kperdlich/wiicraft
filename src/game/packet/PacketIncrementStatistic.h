#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

class PacketIncrementStatistic : public Packet
{
public:
    PacketIncrementStatistic()
        : Packet(PACKET_INCREMENT_STATISTICS)
    {
    }

    void Read(const net::Socket& socket) override
    {
        m_StatisticID = socket.Read<int32_t>();
        m_Amount = socket.Read<char>();
    }
    void Action() override
    {
    }
    Packet* CreateInstance() const override
    {
        return new PacketIncrementStatistic();
    }

protected:
    void SendContent(const net::Socket& socket) const override
    {
    }

    int32_t m_StatisticID = 0;
    char m_Amount = 0;
};
