#pragma once

#include "Core.h"
#include "Packet.h"
#include "PacketGlobals.h"

// todo implement

class PacketEntityAction : public Packet
{
public:
    PacketEntityAction()
        : Packet(PACKET_ENTITY_ACTION)
    {
    }

    void Read(const net::Socket& socket) override
    {
        mEID = socket.Read<int32_t>();
        mActionID = socket.Read<int8_t>();
    }

    void Action() override
    {
    }
    Packet* CreateInstance() const override
    {
        return new PacketEntityAction();
    }

protected:
    void SendContent(const net::Socket& socket) const override
    {
    }
    int32_t mEID;
    int8_t mActionID;
};
