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

#include <string>
#include "Packet.h"
#include "PacketGlobals.h"
#include "globals.h"
#include "eventmanager.h"
#include "EventDataServerConnected.h"
#include "core.h"


class PacketLogin : public Packet
{
public:
    PacketLogin() : Packet(PACKET_LOGIN) {}
    PacketLogin(const std::string playerName)
        : Packet(PACKET_LOGIN), m_ProtocolVersion(SERVER_PROTOCOL_VERSION), m_PlayerName(playerName) {}

    void Read(const net::Socket &socket) override
    {        
        m_ProtocolVersion = socket.Read<int32_t>();
        socket.Read<int16_t>(); // read unused empty string
        m_LevelType     = socket.ReadString();
        m_ServerMode    = socket.Read<int32_t>();
        m_Dimension     = socket.Read<int32_t>();
        m_Difficulty    = socket.Read<char>();
        m_Vanilla       = socket.Read<unsigned char>();
        m_MaxPlayers    = socket.Read<unsigned char>();
    }

    void Action() override
    {
        core::IEventManager::Get()->TriggerEvent(std::make_shared<wiicraft::EventDataServerConnected>());        
    }

    Packet* CreateInstance() const override
    {
       return new PacketLogin();
    }

protected:
    void SendContent(const net::Socket& socket) const override
    {
        socket.Send<int32_t>(m_ProtocolVersion);
        socket.Send<int16_t>((int16_t)m_PlayerName.length());
        socket.SendStringAsUtf16(m_PlayerName);
        for(uint32_t i = 0; i < 13; ++i)
             socket.Send<char>(0x00);
    }
private:
    std::string m_PlayerName;
    std::string m_LevelType;
    int32_t m_ProtocolVersion;    
    int32_t m_ServerMode;
    int32_t m_Dimension;
    char m_Difficulty;
    unsigned char m_Vanilla;
    unsigned char m_MaxPlayers;
};
