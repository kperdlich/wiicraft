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

#include <sstream>
#include <network.h>
#include "networkManager.h"
#include "core.h"
#include "packet/PacketHandshake.h"
#include "Thread.h"
#include "SafeQueue.h"
#include "PacketChatMessage.h"

void wiicraft::NetworkManager::Init()
{
	m_bInitialized = if_config(m_LocalIP, m_Gateway, m_Netmask, true, 5) >= 0;
	if (m_bInitialized)
	{
        //LOG("Network configured, ip: %s, gw: %s, mask %s", m_LocalIP, m_Gateway, m_Netmask);
		int initStatus = net_init();
        ASSERT(initStatus == 0);
	}
	else
	{
        ASSERT(false);
        //ERROR("Network configuration failed!");
	}
}

void wiicraft::NetworkManager::Connect(const std::string& ip, uint16_t port)
{
	if (m_bInitialized)
    {
        ASSERT(!m_ServerConnection);
        m_ServerConnection = std::make_unique<wiicraft::ServerConnection>(ip, port);
        m_ServerConnection->Connect();
    }
}

void wiicraft::NetworkManager::Destroy()
{
	if (m_bInitialized)
	{
        m_ServerConnection->Destroy();
		//net_deinit();
	}
}

void wiicraft::NetworkManager::Update()
{
    if (!m_bInitialized || !m_ServerConnection)
        return;

	for (uint16_t i = 0; i < 10; ++i)
	{
        Packet* p = m_ServerConnection->PopPacket();
		if (p)
		{
			p->Action();            
			delete p;
		}
	}
}
