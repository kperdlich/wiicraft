#pragma once

#include "Packet.h"
#include "PacketGlobals.h"
#include "eventmanager.h"
#include "EventDataEntityRelativeMove.h"

class PacketEntityLookAndRelativeMove : public Packet
{
public:
    PacketEntityLookAndRelativeMove() : Packet(PACKET_ENTITY_LOOK_RELATIVE_MOVE) {}

    void Read(const net::Socket &socket) override
    {
        m_EID = socket.Read<int32_t>();
        m_DX = socket.Read<int8_t>();
        m_DY = socket.Read<int8_t>();
        m_DZ = socket.Read<int8_t>();
        m_Yaw = socket.Read<int8_t>();
        m_Pitch = socket.Read<int8_t>();
    }

    void Action() override
    {
        core::IEventManager::Get()->TriggerEvent(std::make_shared<wiicraft::EventDataEntityRelativeMove>(m_EID, m_Yaw, m_Pitch, m_DY, m_DY, m_DZ));
    }

    Packet *CreateInstance() const override
    {
        return new PacketEntityLookAndRelativeMove();
    }

protected:
    void SendContent(const net::Socket &socket) const override
    {
    }

    int32_t m_EID = 0;
    int8_t m_Yaw = 0, m_Pitch = 0;
    int8_t m_DX = 0, m_DY = 0, m_DZ = 0;
};
