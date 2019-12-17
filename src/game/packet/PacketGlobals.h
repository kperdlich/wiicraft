/***
 *
 * Copyright (C) 2018 DaeFennek
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
***/

#pragma once

#include <stdint.h>

constexpr uint32_t PACKET_KEEP_ALIVE                = 0x00;
constexpr uint32_t PACKET_LOGIN                     = 0x01;
constexpr uint32_t PACKET_HANDSHAKE                 = 0x02;
constexpr uint32_t PACKET_CHAT_MESSAGE              = 0x03;
constexpr uint32_t PACKET_TIME_UPDATE               = 0x04;
constexpr uint32_t PACKET_ENTITY_EQUIPMENT          = 0x05;
constexpr uint32_t PACKET_SPAWN_POSITION            = 0x06;
constexpr uint32_t PACKET_USE_ENTITY                = 0x07;
constexpr uint32_t PACKET_UPDATE_HEALTH             = 0x08;
constexpr uint32_t PACKET_RESPAWN                   = 0x09;
constexpr uint32_t PACKET_PLAYER                    = 0x0A;
constexpr uint32_t PACKET_PLAYER_POSITION           = 0x0B;
constexpr uint32_t PACKET_PLAYER_LOOK               = 0x0C;
constexpr uint32_t PACKET_PLAYER_POSITION_AND_LOOK  = 0x0D;
constexpr uint32_t PACKET_PLAYER_DIGGING            = 0x0E;
constexpr uint32_t PACKET_PLAYER_BLOCK_PLACEMENT    = 0x0F;
constexpr uint32_t PACKET_HELD_ITEM_CHANGE          = 0x10;
constexpr uint32_t PACKET_USE_BED                   = 0x11;
constexpr uint32_t PACKET_ANIMATION                 = 0x12;
constexpr uint32_t PACKET_ENTITY_ACTION             = 0x13;
constexpr uint32_t PACKET_SPAWN_NAMED_ENTITY        = 0x14;
constexpr uint32_t PACKET_SPAWN_DROPPED_ITEM        = 0x15;
constexpr uint32_t PACKET_COLLECT_ITEM              = 0x16;
constexpr uint32_t PACKET_SPAWN_OBJECT              = 0x17;
constexpr uint32_t PACKET_SPAWN_MOB                 = 0x18;
constexpr uint32_t PACKET_SPAWN_PAINTING            = 0x19;
constexpr uint32_t PACKET_SPAWN_EXPERIENCE_ORB      = 0x1A;
constexpr uint32_t PACKET_ENTITY_VELOCITY           = 0x1C;
constexpr uint32_t PACKET_DESTROY_ENTITY            = 0x1D;
constexpr uint32_t PACKET_ENTITY                    = 0x1E;
constexpr uint32_t PACKET_ENTITY_RELATIVE_MOVE      = 0x1F;
constexpr uint32_t PACKET_ENTITY_LOOK               = 0x20;
constexpr uint32_t PACKET_ENTITY_LOOK_RELATIVE_MOVE = 0x21;
constexpr uint32_t PACKET_ENTITY_TELEPORT           = 0x22;
constexpr uint32_t PACKET_ENTITY_HEAD_LOOK          = 0x23;
constexpr uint32_t PACKET_ENTITY_STATUS             = 0x26;
constexpr uint32_t PACKET_ATTACH_ENTITY             = 0x27;
constexpr uint32_t PACKET_ENTITY_METADATA           = 0x28;
constexpr uint32_t PACKET_ENTITY_EFFECT             = 0x29;
constexpr uint32_t PACKET_REMOVE_ENTITY_EFFECT      = 0x2A;
constexpr uint32_t PACKET_SET_EXPERIENCE            = 0x2B;
constexpr uint32_t PACKET_CHUNK_ALLOCATION          = 0x32;
constexpr uint32_t PACKET_CHUNK_DATA                = 0x33;
constexpr uint32_t PACKET_MULTI_BLOCK_CHANGE        = 0x34;
constexpr uint32_t PACKET_BLOCK_CHANGE              = 0x35;
constexpr uint32_t PACKET_BLOCK_ACTION              = 0x36;
constexpr uint32_t PACKET_EXPLOSION                 = 0x3C;
constexpr uint32_t PACKET_SOUND_EFFECT              = 0x3D;
constexpr uint32_t PACKET_CHANGE_GAME_STATE         = 0x46;
constexpr uint32_t PACKET_THUNDERBOLT               = 0x47;
constexpr uint32_t PACKET_OPEN_WINDOW               = 0x64;
constexpr uint32_t PACKET_CLOSE_WINDOW              = 0x65;
constexpr uint32_t PACKET_CLICK_WINDOW              = 0x66;
constexpr uint32_t PACKET_SET_SLOT                  = 0x67;
constexpr uint32_t PACKET_SET_WINDOW_ITEMS          = 0x68;
constexpr uint32_t PACKET_UPDATE_WINDOW_PROPERTY    = 0x69;
constexpr uint32_t PACKET_CONFIRM_TRANSACTION       = 0x6A;
constexpr uint32_t PACKET_CREATIVE_INVENTORY_ACTION = 0x6B;
constexpr uint32_t PACKET_ENCHANT_ITEM              = 0x6C;
constexpr uint32_t PACKET_UPDATE_SIGN               = 0x82;
constexpr uint32_t PACKET_ITEM_DATA                 = 0x83;
constexpr uint32_t PACKET_UPDATE_TILE_ENTITY        = 0x84;
constexpr uint32_t PACKET_INCREMENT_STATISTICS      = 0xC8;
constexpr uint32_t PACKET_PLAYER_LIST_ITEM          = 0xC9;
constexpr uint32_t PACKET_PLAYER_ABILITIES          = 0xCA;
constexpr uint32_t PACKET_PLUGIN_MESSAGE            = 0xFA;
constexpr uint32_t PACKET_SERVER_LIST_PING          = 0xFE;
constexpr uint32_t PACKET_DISCONNECT                = 0xFF;
