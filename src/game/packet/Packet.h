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

#include <cstring>
#include <vector>
#include <cstdio>
#include "socket.h"

struct SlotData
{
	int16_t BlockID;
    int8_t ItemCount;
	int16_t ItemDamage;
    int8_t NBT;
};


class Packet
{
public:
	explicit Packet(unsigned char id) : m_ID(id) {}
	virtual ~Packet() {}
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

    void ReadSlotData(std::vector<SlotData>& slotData, int16_t count, const net::Socket& socket) const
	{
		slotData.clear();
		SlotData data;
		for (int16_t i = 0; i < count; ++i)
		{
			ReadSlotData(data, socket);
			slotData.emplace_back(data);
		}
	}

    void ReadSlotData(SlotData& data, const net::Socket& socket) const
	{
		memset(&data, 0, sizeof(data));
		data.BlockID = socket.Read<int16_t>();
		if (data.BlockID != -1)
		{
			data.ItemCount = socket.Read<char>();
			data.ItemDamage = socket.Read<int16_t>();
			data.NBT = socket.Read<char>(); // todo handle nbt correctly
		}
	}

    uint8_t m_ID;

};
