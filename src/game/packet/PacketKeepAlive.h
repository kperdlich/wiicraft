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

#include "Packet.h"
#include "PacketGlobals.h"

class PacketKeepAlive : public Packet
{
public:
    PacketKeepAlive(int32_t id = 0)
        : Packet(PACKET_KEEP_ALIVE)
        , m_KeepAliveID(id)
    {
    }

    void Read(const net::Socket& socket) override
    {
        m_KeepAliveID = socket.Read<int32_t>();
    }

    void Action() override
    {
        // send id back to server
        Send();
    }

    Packet* CreateInstance() const override
    {
        return new PacketKeepAlive();
    }

protected:
    void SendContent(const net::Socket& socket) const override
    {
        socket.Send<int32_t>(m_KeepAliveID);
    }

private:
    int32_t m_KeepAliveID = 0;
};
