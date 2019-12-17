#pragma once

#include "Packet.h"
#include "PacketGlobals.h"
#include "core.h"
#include "eventmanager.h"
#include "vector3f.h"
#include "EventDataSetPlayerPositionAndLook.h"

class PacketPlayerPositionAndLook : public Packet
{
public:
    PacketPlayerPositionAndLook() : Packet(PACKET_PLAYER_POSITION_AND_LOOK) {}
    PacketPlayerPositionAndLook(double x, double y, double z, float yaw, float pitch, double stance, bool onGround)
        : Packet(PACKET_PLAYER_POSITION_AND_LOOK),
            m_X(x),
            m_Stance(stance),
            m_Y(y),
            m_Z(z),
            m_Yaw(yaw),
            m_Pitch(pitch),
            m_bOnGround(onGround)
    {
    }

    void Read(const net::Socket &socket) override
    {
        m_X = socket.Read<double>();
        m_Stance = socket.Read<double>();
        m_Y = socket.Read<double>();
        m_Z = socket.Read<double>();
        m_Yaw = socket.Read<float>();
        m_Pitch = socket.Read<float>();
        m_bOnGround = socket.Read<bool>();
    }
    void Action() override
    {
        core::IEventManager::Get()->TriggerEvent(
                    std::make_shared<wiicraft::EventDataSetPlayerPositionAndLook>(
                        math::Vector3f(m_X, m_Y, m_Z), m_Yaw, m_Pitch, m_Stance, m_bOnGround));
        Send(); // answer server
    }

    Packet *CreateInstance() const override
    {
        return new PacketPlayerPositionAndLook();
    }

protected:
    void SendContent(const net::Socket &socket) const override
    {
        socket.Send<double>(m_X);
        socket.Send<double>(m_Y);
        socket.Send<double>(m_Stance);
        socket.Send<double>(m_Z);
        socket.Send<float>(m_Yaw);
        socket.Send<float>(m_Pitch);
        socket.Send<bool>(m_bOnGround);
    }

    double m_X = 0, m_Stance = 0, m_Y = 0, m_Z = 0;
    float m_Yaw = 0.0f, m_Pitch = 0.0f;
    bool m_bOnGround = false;

};
