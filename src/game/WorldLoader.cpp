#include "WorldLoader.h"
#include "networkManager.h"
#include "filesystem.h"
#include "PacketHandshake.h"
#include "PacketChatMessage.h"
#include "EventDataSendPlayerPosition.h"
#include "EventDataAllChunksInQueueSerialized.h"


wiicraft::WorldLoader::WorldLoader(const std::string &playerName, const std::string &host, uint16_t port)
    : mPlayerName(playerName),
      mHost(host),
      mPort(port),
      mState(WorldLoaderState::CHECK_WORLD_CACHE)
{
}

wiicraft::WorldLoader::~WorldLoader()
{
    core::IEventManager::Get()->RemoveListener(fastdelegate::MakeDelegate(this, &WorldLoader::OnServerConnected), EventDataServerConnected::EventType);
    core::IEventManager::Get()->RemoveListener(fastdelegate::MakeDelegate(this, &WorldLoader::OnWorldLoaded), EventDataAllChunksInQueueSerialized::EventType);
}

void wiicraft::WorldLoader::Update(renderer::Renderer &renderer)
{    
    switch (mState)
    {
        case WorldLoaderState::CHECK_WORLD_CACHE:
            renderer.DrawText(120, 120, L"Clean old world cache...", renderer::ColorRGBA::RED);
            mState = WorldLoaderState::CLEAN_WORLD_CACHE;
            break;
        case WorldLoaderState::CLEAN_WORLD_CACHE:
            //io::RemoveDirectory(WORLD_PATH);
            mState = WorldLoaderState::TRY_CONNECTING_TO_SERVER;
            break;
        case WorldLoaderState::TRY_CONNECTING_TO_SERVER:
        {
            renderer.DrawText(120, 120, L"Connecting to the server...", renderer::ColorRGBA::RED);
            core::IEventManager::Get()->AddListener(fastdelegate::MakeDelegate(this, &WorldLoader::OnServerConnected), EventDataServerConnected::EventType);
            NetworkManager::Get().Connect(mHost, mPort);
            PacketHandshake hs(mPlayerName, mHost, mPort);
            hs.Send();
            mState = WorldLoaderState::WAIT_FOR_SERVER_LOGIN;
            break;
        }
        case WorldLoaderState::WAIT_FOR_SERVER_LOGIN:
            renderer.DrawText(120, 120, L"Connecting to the server...", renderer::ColorRGBA::RED);
            break;
        case WorldLoaderState::CONNECTED_TO_SERVER:
             renderer.DrawText(120, 120, L"Connected to the server...", renderer::ColorRGBA::RED);
             core::IEventManager::Get()->AddListener(fastdelegate::MakeDelegate(this, &WorldLoader::OnWorldLoaded), EventDataAllChunksInQueueSerialized::EventType);
             core::IEventManager::Get()->TriggerEvent(std::make_shared<EventDataSendPlayerPosition>());
             mState = WorldLoaderState::LOAD_CHUNK_DATA_FROM_SERVER;
            break;
        case WorldLoaderState::LOAD_CHUNK_DATA_FROM_SERVER:
             //renderer.DrawText(120, 120, L"Caching Chunks...", renderer::ColorRGBA::RED);
            //renderer.DrawText(renderer.GetWidth() * .5f, renderer.GetHeight() * .5f, L"Load Chunks...", renderer::ColorRGBA::RED);
            break;
        default:
            break;
    }
}

void wiicraft::WorldLoader::OnServerConnected(core::IEventDataPtr eventData)
{
    //std::shared_ptr<EventDataServerConnected> serverConnectedDta = std::static_pointer_cast<EventDataServerConnected>(eventData);
    //core::IEventManager::Get()->RemoveListener(fastdelegate::MakeDelegate(this, &WorldLoader::OnServerConnected), EventDataServerConnected::EventType);
    mState = WorldLoaderState::CONNECTED_TO_SERVER;   
}

void wiicraft::WorldLoader::OnWorldLoaded(core::IEventDataPtr eventData)
{
    mState = WorldLoaderState::DONE;
}
