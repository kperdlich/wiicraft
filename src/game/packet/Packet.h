/***
 *
 * Copyright (C) 2018 DaeFennek
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***/

#pragma once

#include "Core.h"
#include "SlotData.h"
#include "Socket.h"
#include <cstdio>
#include <cstring>
#include <vector>

class Packet
{
public:
    explicit Packet(unsigned char id)
        : m_ID(id)
    {
    }
    virtual ~Packet()
    {
    }
    Packet(const Packet&) = delete;
    Packet(Packet&&) = delete;
    void operator=(const Packet&) = delete;
    void operator=(Packet&&) = delete;

    void Send() const;
    virtual void Read(const net::Socket& socket) = 0;
    virtual void Action() = 0;
    virtual Packet* CreateInstance() const = 0;

    inline uint8_t GetPacketID() const
    {
        return m_ID;
    }

protected:
    virtual void SendContent(const net::Socket& socket) const = 0;

    void ReadSlotData(std::vector<wiicraft::SlotData>& slotData, int16_t count, const net::Socket& socket) const
    {
        slotData.clear();
        wiicraft::SlotData data;
        for (int16_t i = 0; i < count; ++i)
        {
            ReadSlotData(data, socket);
            slotData.emplace_back(data);
        }
    }

    void ReadSlotData(wiicraft::SlotData& data, const net::Socket& socket) const
    {
        memset(&data, 0, sizeof(data));
        data.ItemID = socket.Read<int16_t>();
        if (data.ItemID != -1)
        {
            data.ItemCount = socket.Read<int8_t>();
            data.ItemDamage = socket.Read<int16_t>();
            ASSERT_TEXT(data.ItemDamage == 0, "NBT not implemented!!!");
        }
    }

    uint8_t m_ID;
};
