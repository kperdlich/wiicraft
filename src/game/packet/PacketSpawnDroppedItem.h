#pragma once

#include "Packet.h"
#include "PacketGlobals.h"
#include "eventmanager.h"
#include "EventDataSpawnItemEntity.h"

class PacketSpawnDroppedItem : public Packet
{
public:
    PacketSpawnDroppedItem() : Packet(PACKET_SPAWN_DROPPED_ITEM) {}

    void Read(const net::Socket &socket) override
    {
        m_EID = socket.Read<int32_t>();
        m_Item = socket.Read<int16_t>();
        m_Count = socket.Read<int8_t>();
        m_Data = socket.Read<int16_t>();
        m_X = socket.Read<int32_t>();
        m_Y = socket.Read<int32_t>();
        m_Z = socket.Read<int32_t>();
        m_Rotation = socket.Read<int8_t>();
        m_Pitch = socket.Read<int8_t>();
        m_Roll = socket.Read<int8_t>();
    }

    void Action() override
    {
        core::IEventManager::Get()->TriggerEvent(std::make_shared<wiicraft::EventDataSpawnItemEntity>(m_EID, m_X, m_Y, m_Z, m_Rotation, m_Pitch));

    }

    Packet *CreateInstance() const override
    {
        return new PacketSpawnDroppedItem();
    }

protected:
    void SendContent(const net::Socket &socket) const override {}

    int32_t m_EID = 0;
    int16_t m_Item = 0;
    int8_t m_Count = 0;
    int16_t m_Data = 0;
    int32_t m_X = 0, m_Y = 0, m_Z = 0;
    int8_t m_Rotation = 0;
    int8_t m_Pitch = 0;
    int8_t m_Roll = 0;

};

