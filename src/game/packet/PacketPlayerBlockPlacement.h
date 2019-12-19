#pragma once

#include "Packet.h"
#include "PacketGlobals.h"
#include "slotdata.h"

// todo implement

class PacketPlayerBlockPlacement : public Packet
{
public:
    PacketPlayerBlockPlacement() : Packet(PACKET_PLAYER_BLOCK_PLACEMENT) {}
    PacketPlayerBlockPlacement(int32_t x, uint8_t y, int32_t z, int8_t direction, int8_t blockType)
        : Packet(PACKET_PLAYER_BLOCK_PLACEMENT),
          mX(x),
          mZ(z),
          mDirection(direction),
          mY(y)
    {
        memset(&mSlotData, 0, sizeof(wiicraft::SlotData));
        mSlotData.ItemCount = 1;
        mSlotData.ItemID = blockType;
    }

    void Read(const net::Socket &socket) override
    {

    }
    void Action() override
    {

    }
    Packet *CreateInstance() const override
    {
        return new PacketPlayerBlockPlacement();
    }

protected:
    void SendContent(const net::Socket &socket) const override
    {
        socket.Send<int32_t>(mX);
        socket.Send<uint8_t>(mY);
        socket.Send<int32_t>(mZ);
        socket.Send<int8_t>(mDirection);
        socket.Send<int16_t>(mSlotData.ItemID);
        socket.Send<int8_t>(mSlotData.ItemCount);
        socket.Send<int16_t>(0);
    }    

    wiicraft::SlotData mSlotData;
    int32_t mX, mZ;
    int8_t mDirection;
    uint8_t mY;
};
