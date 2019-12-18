#pragma once

#include "Packet.h"
#include "PacketGlobals.h"
#include "eventmanager.h"
#include "EventDataEntityRelativeMove.h"

class PacketEntityRelativeMove : public Packet
{
public:
    PacketEntityRelativeMove() : Packet(PACKET_ENTITY_RELATIVE_MOVE) {}

    void Read(const net::Socket &socket) override
    {
        m_EID = socket.Read<int32_t>();
        m_DX = socket.Read<int8_t>();
        m_DY = socket.Read<int8_t>();
        m_DZ = socket.Read<int8_t>();
    }

    void Action() override
    {
        core::IEventManager::Get()->TriggerEvent(std::make_shared<wiicraft::EventDataEntityRelativeMove>(m_EID, 0, 0, m_DX, m_DY, m_DZ));
    }

    Packet *CreateInstance() const override
    {
        return new PacketEntityRelativeMove();
    }

protected:
    void SendContent(const net::Socket &socket) const override
    {
    }

    int32_t m_EID;
    int8_t m_DX, m_DY, m_DZ;
};
