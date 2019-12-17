#include "WorldLoader.h"
#include "networkManager.h"
#include "filesystem.h"
#include "PacketHandshake.h"
#include "EventDataSendPlayerPosition.h"


wiicraft::WorldLoader::~WorldLoader()
{
    core::IEventManager::Get()->RemoveListener(fastdelegate::MakeDelegate(this, &WorldLoader::OnServerConnected), EventDataServerConnected::EventType);
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
            io::RemoveDirectory(WORLD_PATH);
            mState = WorldLoaderState::TRY_CONNECTING_TO_SERVER;
            break;
        case WorldLoaderState::TRY_CONNECTING_TO_SERVER:
        {
            core::IEventManager::Get()->AddListener(fastdelegate::MakeDelegate(this, &WorldLoader::OnServerConnected), EventDataServerConnected::EventType);
            //EventManager::AddListener(this, EVENT_SERVER_CONNECTION_FAILED);
            const std::string& host = "192.168.0.144";
            const std::string& playerName = "DaeFennek";
            const uint16_t port = 25565;
            NetworkManager::Get().Connect(host, port);

            PacketHandshake hs(playerName, host, port);
            hs.Send();
            mState = WorldLoaderState::WAIT_FOR_SERVER_LOGIN;
            break;
        }
        case WorldLoaderState::WAIT_FOR_SERVER_LOGIN:
            renderer.DrawText(120, 120, L"Connecting to the server...", renderer::ColorRGBA::RED);
            break;
        case WorldLoaderState::CONNECTED_TO_SERVER:
             renderer.DrawText(120, 120, L"Connected to the server...", renderer::ColorRGBA::RED);
             core::IEventManager::Get()->TriggerEvent(std::make_shared<EventDataSendPlayerPosition>());
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
