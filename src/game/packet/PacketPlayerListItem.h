#pragma once

#include "EventDataUpdatePlayerListItem.h"
#include "EventManager.h"
#include "Packet.h"
#include "PacketGlobals.h"

class PacketPlayerListItem : public Packet
{
public:
    PacketPlayerListItem()
        : Packet(PACKET_PLAYER_LIST_ITEM)
    {
    }

    void Read(const net::Socket& socket) override
    {
        m_PlayerName = socket.ReadString();
        m_bOnline = socket.Read<bool>();
        m_Ping = socket.Read<int16_t>();
    }
    void Action() override
    {
        core::IEventManager::Get()->TriggerEvent(std::make_shared<wiicraft::EventDataUpdatePlayerListItem>(m_PlayerName, m_Ping));
    }

    Packet* CreateInstance() const override
    {
        return new PacketPlayerListItem();
    }

protected:
    void SendContent(const net::Socket& socket) const override
    {
    }

    std::string m_PlayerName;
    int16_t m_Ping;
    bool m_bOnline;
};
