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

#include "packet/Packet.h"
#include "NetworkManager.h"

void Packet::Send() const
{
    const net::Socket& socket = wiicraft::NetworkManager::Get().GetSocket();
    if (socket.IsConnected())
    {
        // LOG("Send Packet: %d", m_ID);
        socket.Send<char>(m_ID);
        SendContent(socket);
    }
    else
    {
        ASSERT(false);
    }
}
