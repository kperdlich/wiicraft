#include "ServerConnection.h"
#include "Core.h"
#include "packet/Packet.h"
#include "packet/PacketAnimation.h"
#include "packet/PacketAttachEntity.h"
#include "packet/PacketBlockAction.h"
#include "packet/PacketBlockChange.h"
#include "packet/PacketChangeGameState.h"
#include "packet/PacketChatMessage.h"
#include "packet/PacketChunkAllocation.h"
#include "packet/PacketChunkData.h"
#include "packet/PacketClickWindow.h"
#include "packet/PacketCloseWindow.h"
#include "packet/PacketCollectItem.h"
#include "packet/PacketConfirmTransaction.h"
#include "packet/PacketCreativeInventoryAction.h"
#include "packet/PacketDestroyEntity.h"
#include "packet/PacketDisconnect.h"
#include "packet/PacketEnchantItem.h"
#include "packet/PacketEntity.h"
#include "packet/PacketEntityAction.h"
#include "packet/PacketEntityEffect.h"
#include "packet/PacketEntityEquipment.h"
#include "packet/PacketEntityHeadLook.h"
#include "packet/PacketEntityLook.h"
#include "packet/PacketEntityLookAndRelativeMove.h"
#include "packet/PacketEntityMetadata.h"
#include "packet/PacketEntityRelativeMove.h"
#include "packet/PacketEntityStatus.h"
#include "packet/PacketEntityTeleport.h"
#include "packet/PacketEntityVelocity.h"
#include "packet/PacketExplosion.h"
#include "packet/PacketGlobals.h"
#include "packet/PacketHandshake.h"
#include "packet/PacketHeldItemChange.h"
#include "packet/PacketIncrementStatistic.h"
#include "packet/PacketItemData.h"
#include "packet/PacketKeepAlive.h"
#include "packet/PacketLogin.h"
#include "packet/PacketMultiBlockChange.h"
#include "packet/PacketOpenWindow.h"
#include "packet/PacketPlayer.h"
#include "packet/PacketPlayerAbilites.h"
#include "packet/PacketPlayerBlockPlacement.h"
#include "packet/PacketPlayerDigging.h"
#include "packet/PacketPlayerListItem.h"
#include "packet/PacketPlayerLook.h"
#include "packet/PacketPlayerPosition.h"
#include "packet/PacketPlayerPositionAndLook.h"
#include "packet/PacketPluginMessage.h"
#include "packet/PacketRemoveEntityEffect.h"
#include "packet/PacketRespawn.h"
#include "packet/PacketServerListPing.h"
#include "packet/PacketSetExperience.h"
#include "packet/PacketSetSlot.h"
#include "packet/PacketSetWindowItems.h"
#include "packet/PacketSoundEffect.h"
#include "packet/PacketSpawnDroppedItem.h"
#include "packet/PacketSpawnExperienceOrb.h"
#include "packet/PacketSpawnMob.h"
#include "packet/PacketSpawnNamedEntity.h"
#include "packet/PacketSpawnObject.h"
#include "packet/PacketSpawnPainting.h"
#include "packet/PacketSpawnPosition.h"
#include "packet/PacketThunderbolt.h"
#include "packet/PacketTimeUpdate.h"
#include "packet/PacketUpdateHealth.h"
#include "packet/PacketUpdateSign.h"
#include "packet/PacketUpdateTileEntity.h"
#include "packet/PacketUpdateWindowProperty.h"
#include "packet/PacketUseBed.h"
#include "packet/PacketUseEntity.h"
#include <string>

std::map<unsigned char, Packet*> wiicraft::ServerConnection::s_PacketMap;

wiicraft::ServerConnection::ServerConnection(const std::string& ip, uint16_t port)
    : m_socket(ip, port)
{
    InitPacketMap();
}

void wiicraft::ServerConnection::InitPacketMap()
{
    if (!s_PacketMap.empty())
        return;

    // Register all s2c packets here:
    s_PacketMap[PACKET_KEEP_ALIVE] = new PacketKeepAlive();
    s_PacketMap[PACKET_HANDSHAKE] = new PacketHandshake();
    s_PacketMap[PACKET_LOGIN] = new PacketLogin();
    s_PacketMap[PACKET_SPAWN_POSITION] = new PacketSpawnPosition();
    s_PacketMap[PACKET_PLAYER_ABILITIES] = new PacketPlayerAbilites();
    s_PacketMap[PACKET_TIME_UPDATE] = new PacketTimeUpdate();
    s_PacketMap[PACKET_CHAT_MESSAGE] = new PacketChatMessage();
    s_PacketMap[PACKET_CHUNK_ALLOCATION] = new PacketChunkAllocation();
    s_PacketMap[PACKET_SPAWN_DROPPED_ITEM] = new PacketSpawnDroppedItem();
    s_PacketMap[PACKET_ENTITY_EQUIPMENT] = new PacketEntityEquipment();
    s_PacketMap[PACKET_USE_ENTITY] = new PacketUseEntity();
    s_PacketMap[PACKET_UPDATE_HEALTH] = new PacketUpdateHealth();
    s_PacketMap[PACKET_RESPAWN] = new PacketRespawn();
    s_PacketMap[PACKET_PLAYER] = new PacketPlayer();
    s_PacketMap[PACKET_PLAYER_POSITION] = new PacketPlayerPosition();
    s_PacketMap[PACKET_PLAYER_LOOK] = new PacketPlayerLook();
    s_PacketMap[PACKET_PLAYER_POSITION_AND_LOOK] = new PacketPlayerPositionAndLook();
    s_PacketMap[PACKET_PLAYER_DIGGING] = new PacketPlayerDigging();
    // s_PacketMap[PACKET_PLAYER_BLOCK_PLACEMENT] = new PacketPlayerBlockPlacement();
    s_PacketMap[PACKET_HELD_ITEM_CHANGE] = new PacketHeldItemChange();
    s_PacketMap[PACKET_USE_BED] = new PacketUseBed();
    s_PacketMap[PACKET_ANIMATION] = new PacketAnimation();
    s_PacketMap[PACKET_ENTITY_ACTION] = new PacketEntityAction();
    s_PacketMap[PACKET_SPAWN_NAMED_ENTITY] = new PacketSpawnNamedEntity();
    s_PacketMap[PACKET_COLLECT_ITEM] = new PacketCollectItem();
    s_PacketMap[PACKET_SPAWN_OBJECT] = new PacketSpawnObject();
    // s_PacketMap[PACKET_SPAWN_MOB] = new PacketSpawnMob();
    s_PacketMap[PACKET_SPAWN_PAINTING] = new PacketSpawnPainting();
    s_PacketMap[PACKET_SPAWN_EXPERIENCE_ORB] = new PacketSpawnExperienceOrb();
    s_PacketMap[PACKET_ENTITY_VELOCITY] = new PacketEntityVelocity();
    s_PacketMap[PACKET_DESTROY_ENTITY] = new PacketDestroyEntity();
    s_PacketMap[PACKET_ENTITY] = new PacketEntity();
    s_PacketMap[PACKET_ENTITY_RELATIVE_MOVE] = new PacketEntityRelativeMove();
    s_PacketMap[PACKET_ENTITY_LOOK] = new PacketEntityLook();
    s_PacketMap[PACKET_ENTITY_LOOK_RELATIVE_MOVE] = new PacketEntityLookAndRelativeMove();
    s_PacketMap[PACKET_ENTITY_TELEPORT] = new PacketEntityTeleport();
    s_PacketMap[PACKET_ENTITY_HEAD_LOOK] = new PacketEntityHeadLook();
    s_PacketMap[PACKET_ENTITY_STATUS] = new PacketEntityStatus();
    s_PacketMap[PACKET_ATTACH_ENTITY] = new PacketAttachEntity();
    s_PacketMap[PACKET_ENTITY_METADATA] = new PacketEntityMetadata();
    s_PacketMap[PACKET_ENTITY_EFFECT] = new PacketEntityEffect();
    s_PacketMap[PACKET_REMOVE_ENTITY_EFFECT] = new PacketRemoveEntityEffect();
    s_PacketMap[PACKET_SET_EXPERIENCE] = new PacketSetExperience();
    s_PacketMap[PACKET_CHUNK_DATA] = new PacketChunkData();
    s_PacketMap[PACKET_MULTI_BLOCK_CHANGE] = new PacketMultiBlockChange();
    s_PacketMap[PACKET_BLOCK_CHANGE] = new PacketBlockChange();
    s_PacketMap[PACKET_BLOCK_ACTION] = new PacketBlockAction();
    s_PacketMap[PACKET_EXPLOSION] = new PacketExplosion();
    s_PacketMap[PACKET_SOUND_EFFECT] = new PacketSoundEffect();
    s_PacketMap[PACKET_CHANGE_GAME_STATE] = new PacketChangeGameState();
    s_PacketMap[PACKET_THUNDERBOLT] = new PacketThunderbolt();
    s_PacketMap[PACKET_OPEN_WINDOW] = new PacketOpenWindow();
    s_PacketMap[PACKET_CLOSE_WINDOW] = new PacketCloseWindow();
    // s_PacketMap[PACKET_CLICK_WINDOW] = new PacketClickWindow();
    s_PacketMap[PACKET_SET_SLOT] = new PacketSetSlot();
    s_PacketMap[PACKET_SET_WINDOW_ITEMS] = new PacketSetWindowItems();
    s_PacketMap[PACKET_UPDATE_WINDOW_PROPERTY] = new PacketUpdateWindowProperty();
    s_PacketMap[PACKET_CONFIRM_TRANSACTION] = new PacketConfirmTransaction();
    s_PacketMap[PACKET_CREATIVE_INVENTORY_ACTION] = new PacketCreativeInventoryAction();
    s_PacketMap[PACKET_ENCHANT_ITEM] = new PacketEnchantItem();
    s_PacketMap[PACKET_UPDATE_SIGN] = new PacketUpdateSign();
    s_PacketMap[PACKET_ITEM_DATA] = new PacketItemData();
    s_PacketMap[PACKET_UPDATE_TILE_ENTITY] = new PacketUpdateTileEntity();
    s_PacketMap[PACKET_INCREMENT_STATISTICS] = new PacketIncrementStatistic();
    s_PacketMap[PACKET_PLAYER_LIST_ITEM] = new PacketPlayerListItem();
    s_PacketMap[PACKET_PLUGIN_MESSAGE] = new PacketPluginMessage();
    s_PacketMap[PACKET_SERVER_LIST_PING] = new PacketServerListPing();
    s_PacketMap[PACKET_DISCONNECT] = new PacketDisconnect();
}

void wiicraft::ServerConnection::DestroyPacketMap()
{
    for (auto it = s_PacketMap.begin(); it != s_PacketMap.end(); ++it)
        delete it->second;
    s_PacketMap.clear();
}

Packet* wiicraft::ServerConnection::CreatePacketByID(char id)
{
    auto it = s_PacketMap.find(id);
    if (it != s_PacketMap.end())
    {
        return it->second->CreateInstance();
    }

    return nullptr;
}

void wiicraft::ServerConnection::DestroyQueue()
{
    while (!m_queue.IsEmpty())
        delete m_queue.Pop();
}

bool wiicraft::ServerConnection::Connect()
{
    if (m_socket.Connect())
    {
        return Start() == 0;
    }
    return false;
}

void wiicraft::ServerConnection::Destroy()
{
    Stop();
    m_socket.Disconnect();
    DestroyPacketMap();
    DestroyQueue();
}

void wiicraft::ServerConnection::PreExecute()
{
    while (true)
    {
        if (IsStopped())
            break;
        else
            Execute();
    }
}

void wiicraft::ServerConnection::Execute()
{
    static uint8_t lastValidPacket = 0;
    uint8_t packetID = m_socket.Read<uint8_t>();
    Packet* p = CreatePacketByID(packetID);
    if (p && packetID != PACKET_DISCONNECT)
    {
        lastValidPacket = packetID;
        // LOG("Parse packetID %x", packetID);
        p->Read(m_socket);
        m_queue.Push(p);
    }
    else
    {
        // m_Socket.Close();
        if (packetID == PACKET_DISCONNECT)
        {
            ASSERT_TEXT(false, "Disconnected by Server! PacketId: %d, LastValidPacket: %d", packetID, lastValidPacket);
        }
        else
        {
            ASSERT_TEXT(false, "Couldn't find/create instance of packetID %x. Stop Packet reader", packetID);
        }

        Suspend();
    }
}
