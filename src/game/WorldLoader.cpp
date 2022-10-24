#include "WorldLoader.h"
#include "EventDataAllChunksInQueueSerialized.h"
#include "EventDataSendPlayerPosition.h"
#include "Filesystem.h"
#include "LoadingBackground_png.h"
#include "NetworkManager.h"
#include "PacketChatMessage.h"
#include "PacketHandshake.h"
#include "chunkmanager.h"

wiicraft::WorldLoader::WorldLoader(const std::string& playerName, const std::string& host, uint16_t port)
    : mPlayerName(playerName)
    , mHost(host)
    , mPort(port)
    , mState(WorldLoaderState::CHECK_WORLD_CACHE)
{
    mBackgroundImage = std::make_unique<renderer::Image2D>(LoadingBackground_png, LoadingBackground_png_size);
    mBackgroundSprite = std::make_unique<renderer::Sprite>(*mBackgroundImage);
}

wiicraft::WorldLoader::~WorldLoader()
{
    core::IEventManager::Get()->RemoveListener(
        fastdelegate::MakeDelegate(this, &WorldLoader::OnServerConnected), EventDataServerConnected::EventType);
    core::IEventManager::Get()->RemoveListener(
        fastdelegate::MakeDelegate(this, &WorldLoader::OnWorldLoaded), EventDataAllChunksInQueueSerialized::EventType);
}

void wiicraft::WorldLoader::Update(renderer::Renderer& renderer, wiicraft::ChunkManager& chunkManager)
{
    if (mState == WorldLoaderState::DONE)
        return;

    const int32_t centerX = renderer.GetWidth() * 0.5f;
    const int32_t centerY = renderer.GetHeight() * 0.5f;

    mBackgroundSprite->SetPosX(centerX);
    mBackgroundSprite->SetPosY(centerY);
    renderer.Draw(*mBackgroundSprite);

    switch (mState)
    {
    case WorldLoaderState::CHECK_WORLD_CACHE:
        renderer.DrawText(centerX, centerY, L"Preparing World Cache...", renderer::ColorRGBA::WHITE, FTGX_JUSTIFY_CENTER);
        mState = WorldLoaderState::CLEAN_WORLD_CACHE;
        break;
    case WorldLoaderState::CLEAN_WORLD_CACHE:
        io::RemoveDirectory(WORLD_PATH);
        mState = WorldLoaderState::TRY_CONNECTING_TO_SERVER;
        renderer.DrawText(centerX, centerY, L"Connecting To Server...", renderer::ColorRGBA::WHITE, FTGX_JUSTIFY_CENTER);
        break;
    case WorldLoaderState::TRY_CONNECTING_TO_SERVER:
        {
            core::IEventManager::Get()->AddListener(
                fastdelegate::MakeDelegate(this, &WorldLoader::OnServerConnected), EventDataServerConnected::EventType);
            NetworkManager::Get().Connect(mHost, mPort);
            PacketHandshake hs(mPlayerName, mHost, mPort);
            hs.Send();
            mState = WorldLoaderState::WAIT_FOR_SERVER_LOGIN;
            break;
        }
    case WorldLoaderState::WAIT_FOR_SERVER_LOGIN:
        renderer.DrawText(centerX, centerY, L"Waiting For Login...", renderer::ColorRGBA::WHITE, FTGX_JUSTIFY_CENTER);
        break;
    case WorldLoaderState::CONNECTED_TO_SERVER:
        renderer.DrawText(centerX, centerY, L"Connected To Server...", renderer::ColorRGBA::WHITE, FTGX_JUSTIFY_CENTER);
        core::IEventManager::Get()->AddListener(
            fastdelegate::MakeDelegate(this, &WorldLoader::OnWorldLoaded), EventDataAllChunksInQueueSerialized::EventType);
        core::IEventManager::Get()->TriggerEvent(std::make_shared<EventDataSendPlayerPosition>());
        mState = WorldLoaderState::LOAD_CHUNK_DATA_FROM_SERVER;
        break;
    case WorldLoaderState::LOAD_CHUNK_DATA_FROM_SERVER:
        {
            std::wstringstream remainingChunksStream;
            remainingChunksStream << L"Remaining Chunks To Cache: " << chunkManager.GetSerializationQueueCount();
            renderer.DrawText(centerX, centerY, L"Downloading Terrain", renderer::ColorRGBA::WHITE, FTGX_JUSTIFY_CENTER);
            renderer.DrawText(centerX, centerY + 30, remainingChunksStream.str(), renderer::ColorRGBA::WHITE, FTGX_JUSTIFY_CENTER);
            break;
        }
    default:
        break;
    }
}

void wiicraft::WorldLoader::OnServerConnected(core::IEventDataPtr eventData)
{
    mState = WorldLoaderState::CONNECTED_TO_SERVER;
}

void wiicraft::WorldLoader::OnWorldLoaded(core::IEventDataPtr eventData)
{
    mState = WorldLoaderState::DONE;
}
