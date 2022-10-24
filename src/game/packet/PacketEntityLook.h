#pragma once

#include "EventDataEntityRelativeMove.h"
#include "EventManager.h"
#include "Packet.h"
#include "PacketGlobals.h"

class PacketEntityLook : public Packet
{
public:
    PacketEntityLook()
        : Packet(PACKET_ENTITY_LOOK)
    {
    }

    void Read(const net::Socket& socket) override
    {
        m_EID = socket.Read<int32_t>();
        m_Yaw = socket.Read<int8_t>();
        m_Pitch = socket.Read<int8_t>();
    }

    void Action() override
    {
        core::IEventManager::Get()->TriggerEvent(std::make_shared<wiicraft::EventDataEntityRelativeMove>(m_EID, m_Yaw, m_Pitch, 0, 0, 0));
    }

    Packet* CreateInstance() const override
    {
        return new PacketEntityLook();
    }

protected:
    void SendContent(const net::Socket& socket) const override
    {
    }

    int32_t m_EID = 0;
    int8_t m_Yaw = 0, m_Pitch = 0;
};
