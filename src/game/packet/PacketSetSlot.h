#pragma once

#include "Core.h"
#include "EventDataUpdatePlayerHotbarData.h"
#include "EventManager.h"
#include "Packet.h"
#include "PacketGlobals.h"
#include "SlotData.h"
#include <vector>

class PacketSetSlot : public Packet
{
public:
    PacketSetSlot()
        : Packet(PACKET_SET_SLOT)
    {
    }

    void Read(const net::Socket& socket) override
    {
        m_WindowID = socket.Read<int8_t>();
        m_Slot = socket.Read<int16_t>();
        ReadSlotData(m_SlotData, socket);
    }
    void Action() override
    {
        core::IEventManager::Get()->TriggerEvent(
            std::make_shared<wiicraft::EventDataUpdatePlayerHotbarData>(m_WindowID, m_Slot, m_SlotData));
    }

    Packet* CreateInstance() const override
    {
        return new PacketSetSlot();
    }

protected:
    void SendContent(const net::Socket& socket) const override
    {
    }
    wiicraft::SlotData m_SlotData;
    int16_t m_Slot;
    int8_t m_WindowID;
};
