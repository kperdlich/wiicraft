#pragma once

#include "EventManager.h"
#include "Image2d.h"
#include "Renderer.h"
#include "WiiSprite.h"
#include <stdint.h>
#include <string>

namespace wiicraft
{

    enum class WorldLoaderState : int8_t
    {
        CHECK_WORLD_CACHE,
        CLEAN_WORLD_CACHE,
        TRY_CONNECTING_TO_SERVER,
        WAIT_FOR_SERVER_LOGIN,
        CONNECTED_TO_SERVER,
        SERVER_CONNECTION_FAILED,
        LOAD_CHUNK_DATA_FROM_SERVER,
        DONE
    };

    class ChunkManager;

    class WorldLoader
    {
    public:
        WorldLoader(const std::string& playerName, const std::string& host, uint16_t port);
        ~WorldLoader();
        void Update(renderer::Renderer& renderer, wiicraft::ChunkManager& chunkManager);

    private:
        void OnServerConnected(core::IEventDataPtr eventData);
        void OnWorldLoaded(core::IEventDataPtr eventData);

    private:
        std::string mPlayerName;
        std::string mHost;
        std::unique_ptr<renderer::Sprite> mBackgroundSprite;
        std::unique_ptr<renderer::Image2D> mBackgroundImage;
        uint16_t mPort;
        WorldLoaderState mState;
    };

} // namespace wiicraft
