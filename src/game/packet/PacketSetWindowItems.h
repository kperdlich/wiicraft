#pragma once

#include <vector>
#include "Packet.h"
#include "PacketGlobals.h"

class PacketSetWindowItems : public Packet
{
public:
     PacketSetWindowItems() : Packet(PACKET_SET_WINDOW_ITEMS) {}

     ~PacketSetWindowItems()
     {
         m_SlotData.clear();
     }

     void Read(const net::Socket &socket) override
     {
         m_WindowID = socket.Read<char>();
         m_Count = socket.Read<int16_t>();
         ReadSlotData(m_SlotData, m_Count, socket);
     }

     void Action() override
     {
     }
     Packet *CreateInstance() const override
     {
         return new PacketSetWindowItems();
     }

protected:
     void SendContent(const net::Socket &socket) const override
     {
     }

     char m_WindowID = 0;
     int16_t m_Count = 0;
     std::vector<SlotData> m_SlotData;
};
