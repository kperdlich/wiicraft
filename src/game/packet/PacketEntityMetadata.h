#pragma once

#include "Packet.h"
#include "PacketGlobals.h"
#include "slotdata.h"
#include "core.h"

class PacketEntityMetadata : public Packet
{
public:
    PacketEntityMetadata() : Packet(PACKET_ENTITY_METADATA) {}

    void Read(const net::Socket &socket) override
    {
        m_EID = socket.Read<int32_t>();

        int8_t index = 0;
        while ((index = socket.Read<int8_t>()) != 127)
        {
            int32_t type = (index & 0xe0) >> 5;
            switch (type)
            {
                case 0:
                {
                    socket.Read<int8_t>();
                    break;
                }
                case 1:
                {
                    socket.Read<int16_t>();
                    break;
                }
                case 2:
                {
                    socket.Read<int32_t>();
                    break;
                }
                case 3:
                {
                    socket.Read<float>();
                    break;
                }
                case 4:
                {
                    socket.ReadString();
                    break;
                }
                case 5:
                {
                    wiicraft::SlotData slotData;
                    ReadSlotData(slotData, socket);
                    break;
                }
                case 6:
                {
                    socket.Read<int32_t>(); // Chunk Pos
                    socket.Read<int32_t>();
                    socket.Read<int32_t>();
                    break;
                }
                default:
                {
                    ASSERT_TEXT(false, "Unkown Entity Metadata Type: %d, DataType: %d", index, type);
                    break;
                }
            }
        }
    }
    void Action() override
    {
    }
    Packet *CreateInstance() const override
    {
        return new PacketEntityMetadata();
    }

protected:
    void SendContent(const net::Socket &socket) const override
    {
    }

    int32_t m_EID = 0;

};
