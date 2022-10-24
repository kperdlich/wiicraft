#pragma once

#include "Packet.h"
#include "PacketGlobals.h"
#include "Vector3f.h"

class PacketPlayerDigging : public Packet
{
public:
    PacketPlayerDigging()
        : Packet(PACKET_PLAYER_DIGGING)
        , mStatus(0)
        , mY(0)
        , mFace(0)
        , mX(0)
        , mZ(0)
    {
    }
    PacketPlayerDigging(int8_t status)
        : Packet(PACKET_PLAYER_DIGGING)
        , mStatus(status)
        , mY(0)
        , mFace(0)
        , mX(0)
        , mZ(0)
    {
    }

    PacketPlayerDigging(int32_t x, int8_t y, int32_t z, int8_t status, const math::Vector3f& faceNormal)
        : Packet(PACKET_PLAYER_DIGGING)
        , mStatus(status)
        , mY(y)
        , mX(x)
        , mZ(z)
    {
        if (faceNormal.Y() < 0)
        {
            mFace = 0;
        }
        else if (faceNormal.Y() > 0)
        {
            mFace = 1;
        }
        else if (faceNormal.Z() < 0)
        {
            mFace = 2;
        }
        else if (faceNormal.Z() > 0)
        {
            mFace = 3;
        }
        else if (faceNormal.X() < 0)
        {
            mFace = 4;
        }
        else if (faceNormal.X() > 0)
        {
            mFace = 5;
        }
    }

    void Read(const net::Socket& socket) override
    {
    }
    void Action() override
    {
    }
    Packet* CreateInstance() const override
    {
        return new PacketPlayerDigging();
    }

protected:
    void SendContent(const net::Socket& socket) const override
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
