#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

class PacketCollectItem : public Packet
{
public:
    PacketCollectItem() : Packet(PACKET_COLLECT_ITEM) {}

    void Read(const net::Socket &socket) override
    {
        // Send twice by server - Retarded???
        m_CollectedID = socket.Read<int32_t>();
        m_CollectedID = socket.Read<int32_t>();
    }
    void Action() override
    {

    }
    Packet* CreateInstance() const override
    {
        return new PacketCollectItem();
    }

protected:
    void SendContent(const net::Socket &socket) const override
    {

    }

    int32_t m_CollectedID;
};

#pragma once
