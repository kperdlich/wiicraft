#include <network.h>
#include <cstring>
#include "socket.h"
#include "core.h"
#include <debug.h>
net::Socket::Socket(const std::string &host, uint16_t port)
    : mHost(host),
      mPort(port),
      mConnected(false)
{
}

net::Socket::~Socket()
{
	Disconnect();
}

bool net::Socket::Connect()
{
    mSocket = net_socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (mSocket < 0)
	{
        ASSERT(false);
		return false;
	}

	struct sockaddr_in server;
	struct in_addr addr;

    inet_aton(mHost.c_str(), &addr);

	memset(&server, 0, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
    server.sin_port = htons(mPort);
	server.sin_len = sizeof(struct sockaddr_in);
	memcpy((char*)& server.sin_addr, &addr.s_addr, sizeof(struct in_addr));

    int32_t bindingState = net_connect(mSocket, (struct sockaddr*) & server, sizeof(server));
	if (bindingState < 0)
	{	
        ASSERT_TEXT(false, "Could not connect to %s:%d", mHost.c_str(), mPort);
		return false;
	}

    mConnected = true;
	return true;
}

void net::Socket::Disconnect()
{
    if (mConnected)
	{
        mConnected = false;
        net_close(mSocket);
	}
}

void net::Socket::Write(const char* data, size_t size) const
{
	int32_t bytesWrote = 0;
    while (size > 0 && mConnected)
	{
        int32_t ret = net_write(mSocket, data + bytesWrote, size);
		if (ret < 0) // todo handle ret == 0, remote host closed the socket.
		{
            ASSERT_TEXT(ret > 0, "Disconnected by Server");
		}
		else
		{
			size -= ret;
			bytesWrote += ret;
		}
	}
}

void net::Socket::Read(void* data, size_t size) const
{
	int32_t bytesRead = 0;
    while (size > 0 && mConnected)
	{
        int32_t ret = net_read(mSocket, data + bytesRead, size);
		if (ret < 0) // todo handle ret == 0, remote host closed the socket.
		{			
            ASSERT_TEXT(ret > 0, "Disconnected by Server");
        }
		else
		{
			size -= ret;
			bytesRead += ret;
		}
	}
}

void net::Socket::SendStringAsUtf16(const std::string& value) const
{
	for (uint32_t i = 0; i < value.length(); ++i)
	{
		Send<char>(0x00);
		Send<char>(value[i]);
	}
}

void net::Socket::SendString(const std::string& value) const
{
	Send(value.c_str(), value.length());
}

void net::Socket::Send(const char* data, size_t size) const
{
	Write(data, size);
}

std::string net::Socket::ReadString() const
{
	std::string str;
	int16_t len = Read<int16_t>();
	for (int16_t i = 0; i < len; ++i)
	{
		Read<char>(); // 0x00
		str += Read<char>();
	}
	return str;
}
