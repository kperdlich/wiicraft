#pragma once

#include "Core.h"
#include "EventDataSpawnPlayer.h"
#include "EventManager.h"
#include "Packet.h"
#include "PacketGlobals.h"
#include "PacketPlayer.h"
#include "PacketPlayerPosition.h"
#include "Vector3f.h"

class PacketSpawnPosition : public Packet
{
public:
    PacketSpawnPosition()
        : Packet(PACKET_SPAWN_POSITION)
    {
    }

    void Read(const net::Socket& socket) override
    {
        m_x = socket.Read<int32_t>();
        m_y = socket.Read<int32_t>();
        m_z = socket.Read<int32_t>();
    }

    void Action() override
    {
        core::IEventManager::Get()->TriggerEvent(std::make_shared<wiicraft::EventDataSpawnPlayer>(math::Vector3f(m_x, m_y, m_z)));
    }

    Packet* CreateInstance() const override
    {
        return new PacketSpawnPosition();
    }

protected:
    void SendContent(const net::Socket& socket) const override
    {
    }
    int32_t m_x = 0, m_y = 0, m_z = 0;
};
