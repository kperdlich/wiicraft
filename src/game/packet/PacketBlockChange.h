#pragma once

#include "Packet.h"
#include "PacketGlobals.h"
#include "eventmanager.h"
#include "EventDataChangeBlock.h"

class PacketBlockChange : public Packet
{
public:
    PacketBlockChange() : Packet(PACKET_BLOCK_CHANGE) {}

    void Read(const net::Socket &socket) override
    {
        m_X = socket.Read<int32_t>();
        m_Y = socket.Read<int8_t>();
        m_Z = socket.Read<int32_t>();
        m_BlockType = socket.Read<int8_t>();
        m_BlockMetadata = socket.Read<int8_t>();
    }

    void Action() override
    {
        core::IEventManager::Get()->TriggerEvent(std::make_shared<wiicraft::EventDataChangeBlock>(m_X, m_Y, m_Z, m_BlockType));
    }

    Packet* CreateInstance() const override
    {
        return new PacketBlockChange();
    }

protected:
    void SendContent(const net::Socket &socket) const override
    {
    }

    int32_t m_X = 0, m_Z = 0;
    int8_t m_Y = 0, m_BlockType = 0, m_BlockMetadata = 0;
};

