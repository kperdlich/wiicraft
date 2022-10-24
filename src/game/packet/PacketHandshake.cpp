#include "PacketHandshake.h"

std::string PacketHandshake::s_Playername;

PacketHandshake::PacketHandshake()
    : Packet(PACKET_HANDSHAKE)
{
}

PacketHandshake::PacketHandshake(const std::string& playerName, const std::string& ip, uint16_t port)
    : Packet(PACKET_HANDSHAKE)
{
    s_Playername = playerName;
    m_String.clear();
    m_String.append(playerName);
    m_String += ';';
    m_String.append(ip);
    m_String += ':';
    m_String.append(ToString<uint16_t>(port));
}

void PacketHandshake::Read(const net::Socket& socket)
{
    m_String = socket.ReadString();
}

void PacketHandshake::Action()
{
    PacketLogin l(s_Playername);
    l.Send();
}

Packet* PacketHandshake::CreateInstance() const
{
    return new PacketHandshake();
}

void PacketHandshake::SendContent(const net::Socket& socket) const
{
    socket.Send<int16_t>((int16_t)m_String.length());
    socket.SendStringAsUtf16(m_String);
}
