#pragma once

#include "Packet.h"
#include "PacketGlobals.h"
#include "eventmanager.h"
#include "EventDataSpawnEntity.h"

class PacketSpawnNamedEntity : public Packet
{
public:
    PacketSpawnNamedEntity() : Packet(PACKET_SPAWN_NAMED_ENTITY) {}

    void Read(const net::Socket &socket) override
    {
        m_EID = socket.Read<int32_t>();
        m_PlayerName = socket.ReadString();
        m_X = socket.Read<int32_t>();
        m_Y = socket.Read<int32_t>();
        m_Z = socket.Read<int32_t>();
        m_Yaw = socket.Read<int8_t>();
        m_Pitch = socket.Read<int8_t>();
        m_CurrentItem = socket.Read<int16_t>();
    }
    void Action() override
    {
        core::IEventManager::Get()->TriggerEvent(std::make_shared<wiicraft::EventDataSpawnEntity>(
                                                     std::make_shared<wiicraft::Entity>(
                                                         m_EID, m_PlayerName,m_X, m_Y, m_Z, m_CurrentItem, m_Yaw, m_Pitch)));
    }
    Packet *CreateInstance() const override
    {
        return new PacketSpawnNamedEntity();
    }

protected:
    void SendContent(const net::Socket &socket) const override
    {

    }
    std::string m_PlayerName;
    int32_t m_EID = 0;
    int32_t m_X = 0, m_Y = 0, m_Z = 0;
    int16_t m_CurrentItem = 0;
    int8_t m_Yaw = 0, m_Pitch = 0;
};
