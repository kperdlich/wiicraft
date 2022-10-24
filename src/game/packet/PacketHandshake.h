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
#include "PacketLogin.h"
#include "StringHelper.h"
#include <string>

class PacketHandshake : public Packet
{
public:
    PacketHandshake();
    PacketHandshake(const std::string& playerName, const std::string& ip, uint16_t port);
    void Read(const net::Socket& socket) override;
    void Action() override;
    Packet* CreateInstance() const override;

protected:
    void SendContent(const net::Socket& socket) const override;

private:
    static std::string s_Playername;
    std::string m_String;
};
