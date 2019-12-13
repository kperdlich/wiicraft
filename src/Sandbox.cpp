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
#include "staticmesh.h"
#include "frustrum.h"
#include "wii_displaylist.h"
#include <array>
#include <initializer_list>
#include "ClassicBackgroundSprite_png.h"
#include "Minecraft_ttf.h"
#include "rursus_compact_mono_ttf.h"
#include "Cursor_png.h"
#include "Wood_tpl.h"
#include "crosshair_png.h"
#include "core.h"
#include "aabb.h"
#include "raycast.h"
#include "SkyBox.h"
#include "game/chunksection.h"
#include "game/ChunkLoaderMultiplayer.h"
#include "game/BlockManager.h"

void DrawFixedDummy3DTexturedCube(utils::Clock& clock, renderer::Renderer& renderer, math::Matrix3x4& translation, math::Matrix3x4& rotation);
void DrawDummyColorTriangle(utils::Clock& clock);
void DrawDummySprite(renderer::Sprite&, renderer::Renderer &renderer, bool cursor);

struct WPAD_Button {
    uint32_t ButtonDown;
    uint32_t ButtonHeld;
    uint32_t ButtonUp;
} s_wpadButton;

static WPADData* s_wpadData = nullptr;

int main(int argc, char** argv)
{
    ASSERT(fatInitDefault());

    renderer::Renderer renderer(true);
    renderer.SetZModeEnabled(true);
    renderer.SetClearColor(renderer::ColorRGBA::BLACK);
    renderer.SetCullMode(renderer::CullMode::Back);

    renderer::Camera perspectiveCamera(math::Vector3f{.0f, .0f, .1f}, math::Vector3f{.0f, 1.0f, .0f}, math::Vector3f{.0f, .0f, -1.0f}, true);
    perspectiveCamera.SetFrustrum(0.1f, 200.0f, 70.0f, (float) renderer.GetWidth() / (float)renderer.GetHeight());

    renderer::Camera orthographicCamera(math::Vector3f{.0f, .0f, .1f}, math::Vector3f{.0f, 1.0f, .0f}, math::Vector3f{.0f, .0f, .0f}, false);
    orthographicCamera.SetFrustrum(0, renderer.GetHeight(), 0, renderer.GetWidth(), 0, 100.0f);

    utils::Clock clock;
    clock.Start();    

    WPAD_Init();
    WPAD_SetVRes(WPAD_CHAN_0, renderer.GetWidth(), renderer.GetHeight());

    renderer::Image2D backgroundImage(ClassicBackgroundSprite_png);
    renderer::Image2D cursorImage(Cursor_png);
    renderer::Image2D crosshairImage(crosshair_png);
    renderer::Image2D woodImage(Wood_tpl, Wood_tpl_size);    
    std::shared_ptr<renderer::Texture2D> texture = std::make_shared<renderer::Texture2D>(woodImage);

    renderer::Sprite cursorSprite(cursorImage);
    cursorSprite.SetPosX(100.0f);
    cursorSprite.SetPosY(100.0f);
    cursorSprite.SetRotationDeg(180.0f);
    cursorSprite.SetScaleX(0.6f);
    cursorSprite.SetPosZ(-1.0f);

    renderer::Sprite backgroundSprite(backgroundImage);
    backgroundSprite.SetPosX(renderer.GetWidth() * 0.5f);
    backgroundSprite.SetPosY(renderer.GetHeight() * 0.5f);
    backgroundSprite.SetPosZ(-2.0f);

    renderer::Sprite crosshairSprite(crosshairImage);
    crosshairSprite.SetPosX(renderer.GetWidth() * 0.5f);
    crosshairSprite.SetPosY(renderer.GetHeight() * 0.5f);
    crosshairSprite.SetScaleX(0.05f);
    crosshairSprite.SetScaleY(0.05f);

    math::Matrix3x4 translation, rotation;
    translation.SetIdentity();
    rotation.SetIdentity();
    translation.Translate(0.0f, 0.0f, -5.0f);

    math::Vector3f blockPosition = {0.0f, 0.0f, 0.0f};
    const float blockHalfSize = 1.0f;
    guVector vertices[8] = {
                    { (float)blockPosition.X() - blockHalfSize, (float)blockPosition.Y() + blockHalfSize, (float)blockPosition.Z() + blockHalfSize },// v1
                    { (float)blockPosition.X() - blockHalfSize, (float)blockPosition.Y() - blockHalfSize, (float)blockPosition.Z() + blockHalfSize }, //v2
                    { (float)blockPosition.X() + blockHalfSize, (float)blockPosition.Y() - blockHalfSize, (float)blockPosition.Z() + blockHalfSize }, //v3
                    { (float)blockPosition.X() + blockHalfSize, (float)blockPosition.Y() + blockHalfSize, (float)blockPosition.Z() + blockHalfSize }, // v4
                    { (float)blockPosition.X() - blockHalfSize, (float)blockPosition.Y() + blockHalfSize, (float)blockPosition.Z() - blockHalfSize }, //v5
                    { (float)blockPosition.X() + blockHalfSize, (float)blockPosition.Y() + blockHalfSize, (float)blockPosition.Z() - blockHalfSize }, // v6
                    { (float)blockPosition.X() + blockHalfSize, (float)blockPosition.Y() - blockHalfSize, (float)blockPosition.Z() - blockHalfSize }, // v7
                    { (float)blockPosition.X() - blockHalfSize, (float)blockPosition.Y() - blockHalfSize, (float)blockPosition.Z() - blockHalfSize } // v8
                   };


    core::Box pos = {{
        {vertices[0].x, vertices[0].y, vertices[0].z},
        {vertices[1].x, vertices[1].y, vertices[1].z},
        {vertices[2].x, vertices[2].y, vertices[2].z},
        {vertices[3].x, vertices[3].y, vertices[3].z},

        {vertices[4].x, vertices[4].y, vertices[4].z},
        {vertices[5].x, vertices[5].y, vertices[5].z},
        {vertices[6].x, vertices[6].y, vertices[6].z},
        {vertices[7].x, vertices[7].y, vertices[7].z}
       }};

    std::vector<float> tex = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f
    };

    uint32_t color = 0xFFFFFFFF;

    renderer::VertexBuffer modelPos(pos.vertices, sizeof(core::Box), 3 * sizeof(float));
    renderer::VertexBuffer modelTex(tex.data(), tex.size() * sizeof(float), 2 * sizeof(float));
    renderer::VertexBuffer modelColor(&color, sizeof(uint32_t), sizeof(uint32_t));

    renderer::VertexFormat cubeFormat(GX_VTXFMT0);
    cubeFormat.AddAttribute({GX_INDEX16, GX_VA_POS, GX_POS_XYZ, GX_F32});    
    cubeFormat.AddAttribute({GX_INDEX16, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8});
    cubeFormat.AddAttribute({GX_INDEX16, GX_VA_TEX0, GX_TEX_ST, GX_F32});


    std::shared_ptr<renderer::VertexArray> vertexArray = std::make_shared<renderer::VertexArray>(&cubeFormat);
    vertexArray->AddVertexBuffer(GX_VA_POS, &modelPos);
    vertexArray->AddVertexBuffer(GX_VA_CLR0, &modelColor);
    vertexArray->AddVertexBuffer(GX_VA_TEX0, &modelTex);


    std::shared_ptr<renderer::IndexBuffer> indexBuffer =
            std::make_shared<renderer::IndexBuffer>(std::initializer_list<uint16_t>{
                                     0,0,0,
                                     3,0,1,
                                     2,0,3,
                                     1,0,2,

                                     5,0,0,
                                     4,0,1,
                                     7,0,3,
                                     6,0,3,

                                     3,0,0,
                                     5,0,1,
                                     6,0,3,
                                     2,0,2,

                                     4,0,0,
                                     0,0,1,
                                     1,0,3,
                                     7,0,2,

                                     4,0,0,
                                     5,0,1,
                                     3,0,3,
                                     0,0,2,

                                     6,0,0,
                                     7,0,1,
                                     1,0,3,
                                     2,0,2,
                        });

    renderer::StaticMesh cube(indexBuffer, vertexArray, GX_QUADS);
    cube.SetTexture(texture);

    renderer.LoadFont(rursus_compact_mono_ttf, rursus_compact_mono_ttf_size, 20);

    float degree = 1.0f;
    renderer.SetLineWidth(12);
    renderer::SkyBox skybox;    

    wiicraft::BlockManager blockManager;

    wiicraft::ChunkLoaderMultiplayer chunkLoaderJob;
    chunkLoaderJob.Start();

    std::map<std::pair<int32_t, int32_t>, std::shared_ptr<wiicraft::ChunkSection>> chunkSections;
    std::map<std::pair<int32_t, int32_t>, std::shared_ptr<wiicraft::ChunkSection>> chunkCache;

    for (int32_t x = 0; x < 7; ++x)
    {
        for (int32_t y = 0; y < 7; ++y)
        {
            auto cs = std::make_shared<wiicraft::ChunkSection>();
            cs->SetPosition({x, y});            
            //cs->SetTo(wiicraft::BlockType::DIRT);
            chunkSections[cs->GetPositionPair()] = cs;
            //chunkCache[cs->GetPositionPair()] = cs;
        }
    }

    wiicraft::ChunkPosition oldChunkPos = {-1, -1};

    uint64_t millisecondsLastFrame = 0;


    bool drawSpriteRay = false;
    math::Vector3f spriteRayPos;
    math::Vector3f spriteRayForward;
    while(true)
    {
        uint64_t startFrameTime = ticks_to_millisecs(gettime());
        renderer.PreDraw();

        WPAD_ScanPads();
        s_wpadData = WPAD_Data(WPAD_CHAN_0);
        s_wpadButton.ButtonUp = WPAD_ButtonsUp(WPAD_CHAN_0);
        s_wpadButton.ButtonDown = WPAD_ButtonsDown(WPAD_CHAN_0);
        s_wpadButton.ButtonHeld = WPAD_ButtonsHeld(WPAD_CHAN_0);

        if (s_wpadButton.ButtonHeld & WPAD_BUTTON_HOME)
            break;

        math::Matrix3x4 scale;
        scale.SetIdentity();


        if (s_wpadButton.ButtonHeld & WPAD_BUTTON_LEFT)
        {
            //crosshairSprite.SetPosX(crosshairSprite.GetX() -1.0f);
            perspectiveCamera.Rotate(-1.0f, .0f);
            //perspectiveCamera.Move(renderer::CameraMovementDirection::LEFT);
        }

        if (s_wpadButton.ButtonHeld & WPAD_BUTTON_RIGHT)
        {
            //crosshairSprite.SetPosX(crosshairSprite.GetX() + 1.0f);
            perspectiveCamera.Rotate(1.0f, .0f);
            //perspectiveCamera.Move(renderer::CameraMovementDirection::RIGHT);
        }


        if (s_wpadButton.ButtonHeld & WPAD_BUTTON_UP)                    
            perspectiveCamera.Rotate(.0f, 1.0f);
            //perspectiveCamera.Move(renderer::CameraMovementDirection::FORWARD);

        if (s_wpadButton.ButtonHeld & WPAD_BUTTON_DOWN)            
            perspectiveCamera.Rotate(.0f, -1.0f);
            //perspectiveCamera.Move(renderer::CameraMovementDirection::BACKWARD);

        if (s_wpadButton.ButtonHeld & WPAD_BUTTON_A)
        {
            perspectiveCamera.Move(renderer::CameraMovementDirection::FORWARD);
        }

        if (s_wpadButton.ButtonHeld & WPAD_BUTTON_B)
            perspectiveCamera.Move(renderer::CameraMovementDirection::BACKWARD);

        rotation.Rotate('X', degree);
        rotation.Rotate('Y', degree);
        rotation.Rotate('Z', degree);

        // Perspective camera
        renderer.SetCamera(&perspectiveCamera);


        if (s_wpadButton.ButtonDown & WPAD_BUTTON_MINUS)
        {
            drawSpriteRay = true;
            math::Vector3f world = perspectiveCamera.ScreenSpaceToWorldSpace(
                        crosshairSprite.GetX(), crosshairSprite.GetY(),
                        renderer.GetWidth(), renderer.GetHeight());
            spriteRayPos = world;
            spriteRayForward = perspectiveCamera.Forward();
        }


        skybox.Render(renderer);

        renderer.GetCamera()->GenerateFrustrumPlanes(true);

        // Begin - just Sandbox testing stuff
        core::AABB cube1AABB(translation.GetColum(3),
                    {blockHalfSize, blockHalfSize, blockHalfSize});
        const bool render = perspectiveCamera.IsVisible(core::AABB(
                    (translation).GetColum(3),
                    {blockHalfSize, blockHalfSize, blockHalfSize}));
        if (render)
        {
            math::Matrix3x4 scale;
            scale.SetIdentity();

            math::Matrix3x4 viewMatrix = renderer.GetCamera()->GetViewMatrix3x4();
            math::Matrix3x4 modelView = viewMatrix * (translation * rotation * scale);

            renderer.LoadModelViewMatrix(modelView);
            renderer.Draw(cube);
        }

        math::Matrix3x4 cube2Transform;
        cube2Transform.SetIdentity();
        cube2Transform.Translate(-3.0f, 0.0f, -5.0f);
        math::Matrix3x4 modelView2 = renderer.GetCamera()->GetViewMatrix3x4() * cube2Transform;
        renderer.LoadModelViewMatrix(modelView2);
        renderer.Draw(cube);

        renderer.LoadModelViewMatrix(renderer.GetCamera()->GetViewMatrix3x4() * math::Matrix3x4::Identity());
        core::AABB cube2AABB(cube2Transform.GetColum(3), {blockHalfSize, blockHalfSize, blockHalfSize});

        if (cube1AABB.CoolidesWith(cube2AABB))
            renderer.DrawRay(cube2Transform.GetColum(3), math::Vector3f::Up * 2.0f, renderer::ColorRGBA::WHITE);
        else
            renderer.DrawRay(cube2Transform.GetColum(3), math::Vector3f::Up * 2.0f, renderer::ColorRGBA::RED);

        core::RayHitResult result;
        if (core::Raycast({&cube2AABB, &cube1AABB}, {.0f, 0.0f, -1.0f}, math::Vector3f::Forward, 5.0f, result))
        {
            ASSERT(result.Entity == &cube1AABB);
            renderer.DrawRay({.0f, 0.0f, -1.0f}, math::Vector3f::Forward * 5.0f, renderer::ColorRGBA::WHITE);
        }
        else
            renderer.DrawRay({.0f, 0.0f, -1.0f}, math::Vector3f::Forward * 5.0f, renderer::ColorRGBA::RED);
        // End - just Sandbox testing stuff



        if (drawSpriteRay)
        {
            renderer.LoadModelViewMatrix(renderer.GetCamera()->GetViewMatrix3x4() * math::Matrix3x4::Identity());
            renderer.DrawRay(spriteRayPos, spriteRayForward * 10.0f, renderer::ColorRGBA::RED);
        }

        core::RayHitResult focusedBlock;
        math::Vector3f crosshairWorldPosition = perspectiveCamera.ScreenSpaceToWorldSpace(
                    crosshairSprite.GetX(), crosshairSprite.GetY(),
                    renderer.GetWidth(), renderer.GetHeight());
        bool hasBlockInFocus = core::Raycast({&cube2AABB, &cube1AABB}, crosshairWorldPosition, perspectiveCamera.Forward(),  10.0f, focusedBlock);
        if (hasBlockInFocus)
            crosshairSprite.SetColor(renderer::ColorRGBA::RED);
        else
            crosshairSprite.SetColor(renderer::ColorRGBA::WHITE);

        const wiicraft::ChunkPosition currentChunkPos = wiicraft::ChunkSection::WorldPositionToChunkPosition(perspectiveCamera.Position());
        if (currentChunkPos.x != oldChunkPos.x || currentChunkPos.y != oldChunkPos.y)
        {
            chunkCache.clear();
            auto chunkmap = wiicraft::ChunkSection::GenerateChunkMap(perspectiveCamera.Position());
            for (auto chunkMapIt = chunkmap.begin(); chunkMapIt != chunkmap.end();)
            {
                auto chunkIt = chunkSections.find(std::make_pair(chunkMapIt->x, chunkMapIt->y));
                if (chunkIt != chunkSections.end() && chunkIt->second->IsLoaded())
                {
                    chunkCache[chunkIt->first] = chunkIt->second;
                    chunkMapIt = chunkmap.erase(chunkMapIt);
                }
                else
                {
                    ++chunkMapIt;
                }
            }

            std::vector<std::shared_ptr<wiicraft::ChunkSection>> loadingList;
            for (auto& cs : chunkSections)
            {
                auto cacheIt = chunkCache.find(cs.second->GetPositionPair());
                if (cacheIt == chunkCache.end())
                {
                    const wiicraft::ChunkPosition& newChunkPos = chunkmap.back();
                    cs.second->SetTo(wiicraft::BlockType::AIR);
                    cs.second->SetPosition(newChunkPos);
                    cs.second->SetLoaded(false);
                    chunkmap.pop_back();
                    loadingList.push_back(cs.second);
                    chunkLoaderJob.Add(&*cs.second);
                }
            }
            chunkSections.clear();
            for(auto& cs : loadingList)
            {
                chunkSections[cs->GetPositionPair()] = cs;
            }
            for(auto& cs : chunkCache)
            {
                chunkSections[cs.second->GetPositionPair()] = cs.second;
            }
            ASSERT(chunkmap.size() == 0);
            oldChunkPos = currentChunkPos;
        }

        renderer.EnableFog(40.0f, 50.0f, { 192, 216, 255, 0 });
        for (auto& c : chunkCache)
        {            
            auto cs = c.second;
            renderer.LoadModelViewMatrix(renderer.GetCamera()->GetViewMatrix3x4() * math::Matrix3x4::Identity());
            renderer.DrawRay(cs->GetWorldPosition() + math::Vector3f{wiicraft::ChunkSection::CHUNK_SIZE * 0.5f, 256.0f -8.0f,wiicraft::ChunkSection::CHUNK_SIZE * 0.5f}, math::Vector3f::Left * 10.0f, renderer::ColorRGBA::RED);
            renderer.DrawRay(cs->GetWorldPosition() + math::Vector3f{wiicraft::ChunkSection::CHUNK_SIZE * 0.5f, 256.0f-8.0f, wiicraft::ChunkSection::CHUNK_SIZE * 0.5f}, math::Vector3f::Up * 10.0f, renderer::ColorRGBA::GREEN);
            renderer.DrawRay(cs->GetWorldPosition() + math::Vector3f{wiicraft::ChunkSection::CHUNK_SIZE * 0.5f, 256.0f-8.0f, wiicraft::ChunkSection::CHUNK_SIZE * 0.5f}, math::Vector3f::Forward * 10.0f, renderer::ColorRGBA::BLUE);
            cs->Render(renderer, blockManager);
        }
        //renderer.DisableFog();

        // Ortho camera
        renderer.SetCamera(&orthographicCamera);
        DrawDummySprite(cursorSprite, renderer, true);
        DrawDummySprite(crosshairSprite, renderer, false);

        renderer.LoadModelViewMatrix(math::Matrix3x4::Identity());
        math::Vector3f camerapos = perspectiveCamera.Forward();
        std::wstringstream cameraBuffer;
        cameraBuffer << L"CX: ";
        cameraBuffer << camerapos.X();
        cameraBuffer << L" CY: ";
        cameraBuffer << camerapos.Y();
        cameraBuffer << L" CZ: ";
        cameraBuffer << camerapos.Z();
        renderer.DrawText(120, 375, cameraBuffer.str(), renderer::ColorRGBA::RED);

        renderer.LoadModelViewMatrix(math::Matrix3x4::Identity());
        std::wstringstream spriteBuffer;
        spriteBuffer << L"SX: ";
        spriteBuffer << spriteRayPos.X();
        spriteBuffer << L" SY: ";
        spriteBuffer << spriteRayPos.Y();
        spriteBuffer << L" SZ: ";
        spriteBuffer << spriteRayPos.Z();
        renderer.DrawText(120, 395, spriteBuffer.str(), renderer::ColorRGBA::RED);

        /*std::wstringstream str2;
        str2 << L"X: ";
        str2 << cube2Transform.mMtx34[0][3];
        str2 << L" Y: ";
        str2 << cube2Transform.mMtx34[1][3];
        str2 << L" Z: ";
        str2 << cube2Transform.mMtx34[2][3];
        renderer.DrawText(20, 375, str2.str(), renderer::ColorRGBA::RED);*/

        if (render)
            renderer.DrawText(320, 415, L"Cube rendered", renderer::ColorRGBA::RED);
        else
            renderer.DrawText(320, 415, L"Cube not rendered", renderer::ColorRGBA::RED);

        std::wstringstream fps;
        fps << "FPS: ";
        fps << renderer.GetStatistics().FPS;
        renderer.DrawText(380, 30, fps.str(), renderer::ColorRGBA::RED);

        std::wstringstream chunkdisplaylistSize;
        chunkdisplaylistSize << "DP (MB): ";
        chunkdisplaylistSize << renderer.GetStatistics().ChunkDisplayListSizeMB;
        renderer.DrawText(380, 50, chunkdisplaylistSize.str(), renderer::ColorRGBA::RED);

        std::wstringstream chunksInFrustrum;
        chunksInFrustrum << L"Chunks in Frustrum: ";
        chunksInFrustrum << renderer.GetStatistics().ChunksInFrustrum;
        renderer.DrawText(380, 70, chunksInFrustrum.str(), renderer::ColorRGBA::RED);

        std::wstringstream culledChunks;
        culledChunks << L"Culled Chunks: ";
        culledChunks << renderer.GetStatistics().CulledChunks;
        renderer.DrawText(380, 90, culledChunks.str(), renderer::ColorRGBA::RED);

        std::wstringstream queue;
        queue << L"Chunk Queue: ";
        queue << chunkLoaderJob.GetQueueCount();
        renderer.DrawText(380, 110, queue.str(), renderer::ColorRGBA::RED);

        std::wstringstream chunkPos;
        chunkPos << L"Chunk Pos: X:";
        chunkPos << currentChunkPos.x;
        chunkPos << L" Y: ";
        chunkPos << currentChunkPos.y;
        renderer.DrawText(380, 130, chunkPos.str(), renderer::ColorRGBA::RED);

        std::wstringstream oldchunkPos;
        oldchunkPos << L"Old Chunk Pos: X:";
        oldchunkPos << oldChunkPos.x;
        oldchunkPos << L" Y: ";
        oldchunkPos << oldChunkPos.y;
        renderer.DrawText(380, 150, oldchunkPos.str(), renderer::ColorRGBA::RED);

        renderer.DisplayBuffer();
        renderer.ClearStatistics();
        renderer.UpdateFPS();

        millisecondsLastFrame = ticks_to_millisecs(gettime()) - startFrameTime;
    }

    chunkLoaderJob.Stop();
    WPAD_Shutdown();
}

void DrawFixedDummy3DTexturedCube(utils::Clock& clock, renderer::Renderer &renderer, math::Matrix3x4& translation, math::Matrix3x4& rotation)
{

    math::Matrix3x4 modelView;
    math::Matrix3x4 scale;
    scale.SetIdentity();

    if (s_wpadButton.ButtonHeld & WPAD_BUTTON_LEFT)
        rotation.Rotate('Y', -.4f);
    if (s_wpadButton.ButtonHeld & WPAD_BUTTON_RIGHT)
        rotation.Rotate('Y', .4f);
    if (s_wpadButton.ButtonHeld & WPAD_BUTTON_UP)
        rotation.Rotate('X', -.4f);
    if (s_wpadButton.ButtonHeld & WPAD_BUTTON_DOWN)
        rotation.Rotate('X', .4f);
    math::Matrix3x4 viewMatrix = renderer.GetCamera()->GetViewMatrix3x4();

    modelView = viewMatrix * (translation * rotation * scale);

    math::Vector3f blockPosition = {0.0f, 0.0f, 0.0f};
    float blockSize = 1.0f;

    renderer.LoadModelViewMatrix(modelView);
    //GX_LoadPosMtxImm(modelView.mMtx34, GX_PNMTX0);
    guVector vertices[8] = {
                    { (float)blockPosition.X() - blockSize, (float)blockPosition.Y() + blockSize, (float)blockPosition.Z() + blockSize },// v1
                    { (float)blockPosition.X() - blockSize, (float)blockPosition.Y() - blockSize, (float)blockPosition.Z() + blockSize }, //v2
                    { (float)blockPosition.X() + blockSize, (float)blockPosition.Y() - blockSize, (float)blockPosition.Z() + blockSize }, //v3
                    { (float)blockPosition.X() + blockSize, (float)blockPosition.Y() + blockSize, (float)blockPosition.Z() + blockSize }, // v4
                    { (float)blockPosition.X() - blockSize, (float)blockPosition.Y() + blockSize, (float)blockPosition.Z() - blockSize }, //v5
                    { (float)blockPosition.X() + blockSize, (float)blockPosition.Y() + blockSize, (float)blockPosition.Z() - blockSize }, // v6
                    { (float)blockPosition.X() + blockSize, (float)blockPosition.Y() - blockSize, (float)blockPosition.Z() - blockSize }, // v7
                    { (float)blockPosition.X() - blockSize, (float)blockPosition.Y() - blockSize, (float)blockPosition.Z() - blockSize } // v8
                   };



    GX_Begin(GX_QUADS, GX_VTXFMT0, 24);
        // front side
        GX_Position3f32(vertices[0].x, vertices[0].y, vertices[0].z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f, 0.0f);

        GX_Position3f32(vertices[3].x, vertices[3].y, vertices[3].z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f, 0.0f);


        GX_Position3f32(vertices[2].x, vertices[2].y, vertices[2].z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f, 1.0f);


        GX_Position3f32(vertices[1].x, vertices[1].y, vertices[1].z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f, 1.0f);


        // back side
        GX_Position3f32(vertices[5].x, vertices[5].y, vertices[5].z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f, 0.0f);


        GX_Position3f32(vertices[4].x, vertices[4].y, vertices[4].z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f, 0.0f);


        GX_Position3f32(vertices[7].x, vertices[7].y, vertices[7].z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f, 1.0f);


        GX_Position3f32(vertices[6].x, vertices[6].y, vertices[6].z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f, 1.0f);



        // right side
        GX_Position3f32(vertices[3].x, vertices[3].y, vertices[3].z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f, 0.0f);


        GX_Position3f32(vertices[5].x, vertices[5].y, vertices[5].z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f, 0.0f);


        GX_Position3f32(vertices[6].x, vertices[6].y, vertices[6].z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f, 1.0f);


        GX_Position3f32(vertices[2].x, vertices[2].y, vertices[2].z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f, 1.0f);



        // left side
        GX_Position3f32(vertices[4].x, vertices[4].y, vertices[4].z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f, 0.0f);


        GX_Position3f32(vertices[0].x, vertices[0].y, vertices[0].z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f, 0.0f);


        GX_Position3f32(vertices[1].x, vertices[1].y, vertices[1].z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f, 1.0f);


        GX_Position3f32(vertices[7].x, vertices[7].y, vertices[7].z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f, 1.0f);



        // top side
        GX_Position3f32(vertices[4].x, vertices[4].y, vertices[4].z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f, 0.0f);


        GX_Position3f32(vertices[5].x, vertices[5].y, vertices[5].z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f, 0.0f);


        GX_Position3f32(vertices[3].x, vertices[3].y, vertices[3].z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f, 1.0f);


        GX_Position3f32(vertices[0].x, vertices[0].y, vertices[0].z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f, 1.0f);



        // bottom side
        GX_Position3f32(vertices[6].x, vertices[6].y, vertices[6].z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f, 0.0f);


        GX_Position3f32(vertices[7].x, vertices[7].y, vertices[7].z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f, 0.0f);


        GX_Position3f32(vertices[1].x, vertices[1].y, vertices[1].z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f, 1.0f);


        GX_Position3f32(vertices[2].x, vertices[2].y, vertices[2].z);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f, 1.0f);

    GX_End();
}


void DrawDummySprite(renderer::Sprite& sprite, renderer::Renderer& renderer, bool cursor)
{
    if (cursor)
    {
        if (s_wpadButton.ButtonHeld & WPAD_BUTTON_LEFT)
            sprite.Rotate(1.0f);
        if (s_wpadButton.ButtonHeld & WPAD_BUTTON_RIGHT)
            sprite.Rotate(-1.0f);
        if (s_wpadButton.ButtonHeld & WPAD_BUTTON_UP)
            sprite.Translate(0.0f, -1.0f, 0.0f);
        if (s_wpadButton.ButtonHeld & WPAD_BUTTON_DOWN)
            sprite.Translate(0.0f, 1.0f, 0.0f);

    }
    math::Matrix3x4 viewMatrix = renderer.GetCamera()->GetViewMatrix3x4();   
    math::Matrix3x4 translation, scale, rotation, model, modelView;
    translation.SetIdentity();
    scale.SetIdentity();
    rotation.SetIdentity();
    model.SetIdentity();

    scale.Scale(sprite.ScaleX(), sprite.ScaleY(), 0.0f);
    translation.Translate(sprite.GetX(), sprite.GetY(), sprite.GetZ());
    rotation.Rotate('Z', sprite.RotationDeg());

    model = translation * rotation * scale;
    modelView = viewMatrix * model;

    renderer.LoadModelViewMatrix(modelView);
    renderer.Draw(sprite);
}

void DrawDummyColorTriangle(utils::Clock& clock)
{
    static float x = 0, y = 0;
    static float scale = 1;

    if (s_wpadButton.ButtonHeld & WPAD_BUTTON_LEFT)
        x -= .1f;
    if (s_wpadButton.ButtonHeld & WPAD_BUTTON_RIGHT)
        x += .1f;
    if (s_wpadButton.ButtonHeld & WPAD_BUTTON_UP)
        y -= .1f;
    if (s_wpadButton.ButtonHeld & WPAD_BUTTON_DOWN)
        y += .1f;


    GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GX_SetColorUpdate(GX_TRUE);

    // GX_ClearVtxDesc();


    /*GX_SetNumTexGens(0);
    GX_SetNumTevStages(1);
    GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORDNULL, GX_TEXMAP_NULL,
        GX_COLOR0A0);*/

    GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GX_SetNumChans(1);


    if (clock.ElapsedMilliseconds() >= 1000)
    {
        ++x;
        //scale += 0.1f;
        clock.Start();
    }

    Mtx mtx;
    guMtxIdentity(mtx);
    guMtxTrans(mtx, x, 20, 0);

    //guMtxTrans(mtx, -100, -100, 0);
    GX_LoadPosMtxImm(mtx, GX_PNMTX0);
    GX_Begin(GX_TRIANGLES, GX_VTXFMT0, 3);
            GX_Position3f32(100, 300, 0);
            GX_Color4u8(255, 0, 0, 255);

            GX_Position3f32(200, 200, 0);
            GX_Color4u8(0, 255, 0, 255);

            GX_Position3f32(300, 300, 0);
            GX_Color4u8(0, 0, 255, 255);
    GX_End();


}
