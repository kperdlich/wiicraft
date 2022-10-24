#pragma once

#include "Packet.h"
#include "PacketGlobals.h"

class PacketPlayerLook : public Packet
{
public:
    PacketPlayerLook()
        : Packet(PACKET_PLAYER_LOOK)
    {
    }
    PacketPlayerLook(float yaw, float pitch, bool onGround)
        : Packet(PACKET_PLAYER_LOOK)
        , mYaw(yaw)
        , mPitch(pitch)
        , mOnGround(onGround)
    {
    }

    void Read(const net::Socket& socket) override
    {
    }

    void Action() override
    {
    }
    Packet* CreateInstance() const override
    {
        return new PacketPlayerLook();
    }

protected:
    void SendContent(const net::Socket& socket) const override
    {
        socket.Send<float>(mYaw);
        socket.Send<float>(mPitch);
        socket.Send<bool>(mOnGround);
    }
    float mYaw, mPitch;
    bool mOnGround;
};
