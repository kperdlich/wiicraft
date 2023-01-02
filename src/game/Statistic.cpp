#include "Statistic.h"
#include "Camera.h"
#include "Globals.h"
#include <sstream>

void wiicraft::ShowStatisics(renderer::Renderer& renderer, ChunkManager& world, const wiicraft::Player& player)
{
    renderer.LoadModelViewMatrix(renderer.GetCamera()->GetViewMatrix3x4());

    std::wstringstream game;
    game << GAME_NAME;
    renderer.DrawText(30, 30, game.str(), renderer::ColorRGBA::WHITE);

    std::wstringstream pingFps;
    pingFps << "FPS: " << renderer.GetStatistics().FPS << " / Ping: " << player.GetPing() << "ms"
            << " / " << player.GetName().c_str();
    renderer.DrawText(30, 50, pingFps.str(), renderer::ColorRGBA::WHITE);

    std::wstringstream blockPos;
    blockPos << "Focused Block: XYZ: " << player.GetFocusedBlockPosition().X() << "/" << player.GetFocusedBlockPosition().Y() << "/"
             << player.GetFocusedBlockPosition().Z();
    renderer.DrawText(30, 70, blockPos.str(), renderer::ColorRGBA::WHITE);

    std::wstringstream playerAbilities;
    playerAbilities << "IV:" << player.IsInvulnerable() << " / IF:" << player.IsFlying() << " / CF:" << player.CanFly()
                    << " / ID:" << player.IsInstantDestroy();
    renderer.DrawText(30, 90, playerAbilities.str(), renderer::ColorRGBA::WHITE);

    std::wstringstream spriteBuffer;
    spriteBuffer << L"Position XYZ: " << player.GetPosition().X() << L"/" << player.GetPosition().Y() << L"/" << player.GetPosition().Z();
    renderer.DrawText(30, 110, spriteBuffer.str(), renderer::ColorRGBA::WHITE);

    std::wstringstream chunkPos;
    const wiicraft::ChunkPosition currentChunkPos = wiicraft::ChunkSection::WorldPositionToChunkPosition(player.GetPosition());
    chunkPos << L"Chunk: XY: " << currentChunkPos.first << L"/" << currentChunkPos.second;
    renderer.DrawText(30, 130, chunkPos.str(), renderer::ColorRGBA::WHITE);

    std::wstringstream frustrum;
    frustrum << "Frustrum: " << renderer.GetStatistics().ChunksInFrustrum << " / Culled: " << renderer.GetStatistics().CulledChunks;
    renderer.DrawText(30, 150, frustrum.str(), renderer::ColorRGBA::WHITE);

    std::wstringstream chunkdisplaylistSize;
    chunkdisplaylistSize << "DP (MB): " << renderer.GetStatistics().ChunkDisplayListSizeMB;
    renderer.DrawText(30, 170, chunkdisplaylistSize.str(), renderer::ColorRGBA::WHITE);

    std::wstringstream loaderQueue;
    uint32_t loaderQueueCount = world.GetLoaderQueueCount();
    loaderQueue << L"Loader Queue: " << loaderQueueCount;
    renderer.DrawText(30, 190, loaderQueue.str(), loaderQueueCount >= 10 ? renderer::ColorRGBA::RED : renderer::ColorRGBA::WHITE);

    std::wstringstream saveQueue;
    uint32_t saveQueueCount = world.GetSerializationQueueCount();
    saveQueue << L"/ Save Queue: " << saveQueueCount;
    renderer.DrawText(250, 190, saveQueue.str(), saveQueueCount >= 10 ? renderer::ColorRGBA::RED : renderer::ColorRGBA::WHITE);
}
