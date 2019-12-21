#pragma once
#include <string>

namespace net {
class Socket
{
public:
    Socket(const std::string& host, uint16_t port);
	~Socket();
	Socket(const Socket&) = delete;
	Socket(Socket&&) = delete;
	void operator=(const Socket&) = delete;
	void operator=(Socket&&) = delete;

    bool Connect();
	void Disconnect();
	void Write(const char* data, size_t size) const;
	void Read(void* data, size_t size) const;
	void SendStringAsUtf16(const std::string& value) const;
	void SendString(const std::string& value) const;
	void Send(const char* data, size_t size) const;
	std::string ReadString() const;

	template<typename T>
	T Read() const
	{
        T value = 0;
		Read(&value, sizeof(T));
		return value;
	}

	template<typename T>
	void Send(T value) const
	{
		Write((const char*)& value, sizeof(value));
	}

	inline bool IsConnected() const
	{
        return mConnected;
	}

private:
    int32_t mSocket;
    std::string mHost;
    uint16_t mPort;
    bool mConnected = false;
};
}
