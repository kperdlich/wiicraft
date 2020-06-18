/***
 *
 * Copyright (C) 2019 DaeFennek
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

#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include "renderer.h"
#include "mathhelper.h"
#include "clock.h"
#include "colorrgba.h"
#include "camera.h"
#include "vector3f.h"
#include "vertexformat.h"
#include "image2d.h"
#include "texture2d.h"
#include "wii_sprite.h"
#include "indexbuffer.h"
#include "vertexbuffer.h"
#include "vertexarray.h"
#include "vertexformat.h"
#include "mesh.h"
#include "frustrum.h"
#include "wii_displaylist.h"
#include <array>
#include <initializer_list>
#include "Minecraft_ttf.h"
#include "crosshair_png.h"
#include "core.h"
#include "aabb.h"
#include "raycast.h"
#include "SkyBox.h"
#include "wii_pad.h"
#include "filesystem.h"
#include "game/chunksection.h"
#include "game/chunkmanager.h"
#include "game/ChunkLoaderMultiplayer.h"
#include "game/BlockManager.h"
#include "game/player.h"
#include "game/EntityManager.h"
#include "eventmanager.h"
#include "EventDataSpawnPlayer.h"
#include "networkManager.h"
#include "WorldLoader.h"
#include "iniconfig.h"
#include "globals.h"
#include "statistic.h"


int main(int argc, char** argv)
{
    renderer::Renderer renderer(true);
    renderer.LoadFont(Minecraft_ttf, Minecraft_ttf_size, 20);
    renderer.SetLineWidth(12);
    renderer.SetZModeEnabled(true);
    renderer.SetClearColor(renderer::ColorRGBA::BLACK);
    renderer.SetCullMode(renderer::CullMode::Back);

    io::Init();

    core::IniConfig config;
    config.Parse(CONFIG_FILE);

    core::EventManager eventManager(true);

    wiicraft::NetworkManager::Get().Init();
    wiicraft::EntityManager entityManager;

    std::shared_ptr<renderer::Camera> perspectiveCamera = std::make_shared<renderer::Camera>(math::Vector3f{.0f, .0f, .1f}, math::Vector3f{.0f, 1.0f, .0f}, math::Vector3f{.0f, .0f, -1.0f}, true);
    perspectiveCamera->SetFrustrum(0.1f, 200.0f, 70.0f, (float) renderer.GetWidth() / (float)renderer.GetHeight());

    std::shared_ptr<renderer::Camera> orthographicCamera = std::make_shared<renderer::Camera>(math::Vector3f{.0f, .0f, .1f}, math::Vector3f{.0f, 1.0f, .0f}, math::Vector3f{.0f, .0f, .0f}, false);
    orthographicCamera->SetFrustrum(0, renderer.GetHeight(), 0, renderer.GetWidth(), 0, 100.0f);     

    std::shared_ptr<core::WiiPad> pad = std::make_shared<core::WiiPad>(WPAD_CHAN_0, renderer.GetWidth(), renderer.GetHeight());
    renderer::SkyBox skybox;
    wiicraft::ChunkManager chunkManager;
    wiicraft::Player player(perspectiveCamera, pad, config.GetValue<std::string>("Default", "PlayerName"));
    wiicraft::WorldLoader worldLoader(config.GetValue<std::string>("Default", "PlayerName"), config.GetValue<std::string>("MinecraftServer", "Host"),
                                      config.GetValue<uint16_t>("MinecraftServer", "Port"));
    utils::Clock clock;
    uint64_t millisecondsLastFrame = 0;
    bool showDebugStatistics = false;

    SYS_SetResetCallback([](uint32_t, void*){ exit(0); });
    SYS_SetPowerCallback([](){ exit(0); });

    const bool fogEnabled = config.GetValue<bool>("Graphics", "EnableFog");

    while(true)
    {
        clock.Start();
        renderer.PreDraw();

        pad->Update();
        if (pad->ButtonsHeld() & WPAD_BUTTON_HOME)
            break;

        // Perspective camera
        renderer.SetZModeEnabled(true);
        renderer.SetCamera(perspectiveCamera);
        skybox.Render(renderer);

        renderer.GetCamera()->GenerateFrustrumPlanes(true);

        //renderer.DrawRay(perspectiveCamera->Position(), math::Vector3f::Left * 10.0f, renderer::ColorRGBA::RED);
        //renderer.DrawRay(perspectiveCamera->Position(), math::Vector3f::Up * 10.0f, renderer::ColorRGBA::GREEN);
        //renderer.DrawRay(perspectiveCamera->Position(), math::Vector3f::Forward * 10.0f, renderer::ColorRGBA::BLUE);

        //renderer.DrawRay(player.GetPosition(), math::Vector3f::Left * 10.0f, renderer::ColorRGBA::RED);
        //renderer.DrawRay(player.GetPosition(), math::Vector3f::Up * 10.0f, renderer::ColorRGBA::GREEN);
        //renderer.DrawRay(player.GetPosition(), math::Vector3f::Forward * 10.0f, renderer::ColorRGBA::BLUE);


        chunkManager.UpdateChunksAround(player.GetPosition());
        chunkManager.Render(renderer);
        entityManager.Render(renderer);
        player.OnRender3D(millisecondsLastFrame / 1000.0f, renderer, chunkManager);
        //player.DrawAABB(renderer);
        eventManager.TickUpdate();

        if (fogEnabled)
        {
            renderer.EnableFog(20.0f, 30.0f, { 192, 216, 255, 0 });
        }

        // Ortho camera
        renderer.SetCamera(orthographicCamera);
        renderer.SetZModeEnabled(false);
        player.OnRender2D(millisecondsLastFrame / 1000.0f, renderer, chunkManager);

        renderer.LoadModelViewMatrix(orthographicCamera->GetViewMatrix3x4() * math::Matrix3x4::Identity());
        worldLoader.Update(renderer, chunkManager);
        wiicraft::NetworkManager::Get().Update();        

        if (pad->ButtonsDown() & WPAD_BUTTON_UP)
        {
            showDebugStatistics = !showDebugStatistics;
        }

        if (showDebugStatistics)
        {
            wiicraft::ShowStatisics(renderer, chunkManager, player);
        }

        renderer.DisplayBuffer();
        renderer.ClearStatistics();
        renderer.UpdateFPS();

        millisecondsLastFrame = clock.ElapsedMilliseconds();
    }
}
