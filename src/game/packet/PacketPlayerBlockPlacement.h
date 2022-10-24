#pragma once

#include "Packet.h"
#include "PacketGlobals.h"
#include "SlotData.h"
#include "Vector3f.h"

class PacketPlayerBlockPlacement : public Packet
{
public:
    PacketPlayerBlockPlacement()
        : Packet(PACKET_PLAYER_BLOCK_PLACEMENT)
    {
    }
    PacketPlayerBlockPlacement(int32_t x, uint8_t y, int32_t z, const math::Vector3f& faceNormal, const wiicraft::SlotData& slotData)
        : Packet(PACKET_PLAYER_BLOCK_PLACEMENT)
        , mSlotData(slotData)
        , mX(x)
        , mZ(z)
        , mY(y)
    {
        if (faceNormal.Y() < 0)
        {
            mDirection = 0;
        }
        else if (faceNormal.Y() > 0)
        {
            mDirection = 1;
        }
        else if (faceNormal.Z() < 0)
        {
            mDirection = 2;
        }
        else if (faceNormal.Z() > 0)
        {
            mDirection = 3;
        }
        else if (faceNormal.X() < 0)
        {
            mDirection = 4;
        }
        else if (faceNormal.X() > 0)
        {
            mDirection = 5;
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
        return new PacketPlayerBlockPlacement();
    }

protected:
    void SendContent(const net::Socket& socket) const override
    {
        socket.Send<int32_t>(mX);
        socket.Send<uint8_t>(mY);
        socket.Send<int32_t>(mZ);
        socket.Send<int8_t>(mDirection);
        socket.Send<int16_t>(mSlotData.ItemID);
        if (mSlotData.ItemID != -1)
        {
            socket.Send<int8_t>(mSlotData.ItemCount);
            socket.Send<int16_t>(mSlotData.ItemDamage);
        }
    }

    wiicraft::SlotData mSlotData;
    int32_t mX, mZ;
    int8_t mDirection;
    uint8_t mY;
};
