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

#include <map>
#include "socket.h"
#include "Thread.h"
#include "SafeQueue.h"

class Packet;
namespace wiicraft {

class ServerConnection : public core::Thread
{
public:
    ServerConnection(const std::string& ip, uint16_t port);
	ServerConnection(const ServerConnection&) = delete;
	ServerConnection(ServerConnection&&) = delete;
	void operator=(const ServerConnection&) = delete;
	void operator=(ServerConnection&&) = delete;

    bool Connect();
	void Destroy();

    inline const net::Socket& GetSocket() const
	{
		return m_socket;
	}

	inline class Packet* PopPacket()
	{
		if (!m_queue.IsEmpty())
			return m_queue.Pop();
		return nullptr;
	}

private:
	static void DestroyPacketMap();
	static void InitPacketMap();
	static class Packet* CreatePacketByID(char id);
	void DestroyQueue();
protected:
	void Execute() override;
	void PreExecute() override;
private:
    net::Socket m_socket;
    core::SafeQueue<Packet*> m_queue;
    static std::map<unsigned char, Packet*> s_PacketMap;
};
}
