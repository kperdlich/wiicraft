#pragma once

#include <stdint.h>
#include "renderer.h"
#include "eventmanager.h"

namespace wiicraft {

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

class WorldLoader
{
public:
    ~WorldLoader();
    void Update(renderer::Renderer& renderer);
private:
    void OnServerConnected(core::IEventDataPtr eventData);

private:
    WorldLoaderState mState = WorldLoaderState::CHECK_WORLD_CACHE;

};

}
