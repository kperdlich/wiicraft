#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

// todo implement

class PacketPlayerDigging : public Packet
{
public:
    PacketPlayerDigging() : Packet(PACKET_PLAYER_DIGGING) {}

    PacketPlayerDigging(int32_t x, int8_t y, int32_t z, int8_t status, int8_t face) :
        Packet(PACKET_PLAYER_DIGGING),
        mStatus(status),
        mY(y),
        mFace(face),
        mX(x),
        mZ(z)
    {}

    void Read(const net::Socket &socket) override
    {

    }
    void Action() override
    {

    }
    Packet *CreateInstance() const override
    {
        return new PacketPlayerDigging();
    }

protected:
    void SendContent(const net::Socket &socket) const override
    {
        socket.Send<int8_t>(mStatus);
        socket.Send<int32_t>(mX);
        socket.Send<int8_t>(mY);
        socket.Send<int32_t>(mZ);
        socket.Send<int8_t>(mFace);
    }

    int8_t mStatus, mY, mFace;
    int32_t mX, mZ;

};
