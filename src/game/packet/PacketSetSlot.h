#pragma once

#include <vector>
#include "Packet.h"
#include "PacketGlobals.h"
#include "slotdata.h"
#include "eventmanager.h"
#include "EventDataUpdatePlayerHotbarData.h"
#include "core.h"
class PacketSetSlot : public Packet
{
public:
    PacketSetSlot() : Packet(PACKET_SET_SLOT) {}

    void Read(const net::Socket &socket) override
    {
        m_WindowID = socket.Read<int8_t>();
        m_Slot = socket.Read<int16_t>();
        ReadSlotData(m_SlotData, socket);
    }
    void Action() override
    {
        core::IEventManager::Get()->TriggerEvent(std::make_shared<wiicraft::EventDataUpdatePlayerHotbarData>(m_WindowID, m_Slot, m_SlotData));
    }

    Packet *CreateInstance() const override
    {
        return new PacketSetSlot();
    }

protected:
    void SendContent(const net::Socket &socket) const override
    {
    }
    wiicraft::SlotData m_SlotData;
    int16_t m_Slot;
    int8_t m_WindowID;
};
