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

#include <memory>
#include "core.h"
#include "packet/Packet.h"
#include "serverConnection.h"

namespace wiicraft {
class NetworkManager
{
private:
	NetworkManager() {}
public:
	void Init();
	void Connect(const std::string& ip, uint16_t port);
	void Destroy();
	void Update();

	static NetworkManager& Get()
	{
		static NetworkManager s_instance;
		return s_instance;
	}

	inline bool Initialized() const
	{
		return m_bInitialized;
	}

    inline const net::Socket& GetSocket() const
	{
        ASSERT(m_ServerConnection);
        return m_ServerConnection->GetSocket();
	}

	NetworkManager(const NetworkManager&) = delete;
	NetworkManager(NetworkManager&&) = delete;
	void operator=(const NetworkManager&) = delete;
	void operator=(NetworkManager&&) = delete;

private:
	char m_LocalIP[16];
	char m_Gateway[16];
	char m_Netmask[16];
	bool m_bInitialized = false;


    std::unique_ptr<ServerConnection> m_ServerConnection;
};
}
