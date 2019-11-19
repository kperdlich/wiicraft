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

#include "renderer.h"
#include "clock.h"
#include "colorrgba.h"
#include "camera.h"
#include "vector3f.h"
#include "vertexformat.h"
#include "image2d.h"
#include "texture2d.h"
#include "wiisprite.h"
#include "indexbuffer.h"
#include "vertexbuffer.h"
#include "vertexarray.h"
#include "vertexformat.h"
#include "mesh.h"
#include "staticmesh.h"
#include "wiidisplaylist.h"
#include <array>
#include <initializer_list>
#include "ClassicBackgroundSprite_png.h"
#include "Minecraft_ttf.h"
#include "Cursor_png.h"
#include "Wood_tpl.h"
#include <assert.h>

void DrawIndexedDummy3DTexturedCube(utils::Clock& clock, renderer::Renderer& renderer,
                                    math::Matrix3x4& translation, math::Matrix3x4& rotation, renderer::StaticMesh &mesh);
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
    renderer::Renderer renderer(false);
    renderer.SetZModeEnabled(true);
    renderer.SetClearColor(renderer::ColorRGBA::RED);
    renderer.SetCullMode(renderer::CullMode::None);

    renderer::Camera perspectiveCamera(math::Vector3f{.0f, .0f, .1f}, math::Vector3f{.0f, 1.0f, .0f}, math::Vector3f{.0f, .0f, .0f}, true);
    perspectiveCamera.SetFrustrum(0.1f, 200.0f, 70.0f, (float) renderer.GetWidth() / (float)renderer.GetHeight());

    renderer::Camera orthographicCamera(math::Vector3f{.0f, .0f, .1f}, math::Vector3f{.0f, 1.0f, .0f}, math::Vector3f{.0f, .0f, .0f}, false);
    orthographicCamera.SetFrustrum(0, renderer.GetHeight(), 0, renderer.GetWidth(), 0, 100.0f);

    utils::Clock clock;
    clock.Start();

    WPAD_Init();
    WPAD_SetVRes(WPAD_CHAN_0, renderer.GetWidth(), renderer.GetHeight());

    renderer::Image2D backgroundImage(ClassicBackgroundSprite_png);
    renderer::Image2D cursorImage(Cursor_png);
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

    math::Matrix3x4 translation, rotation;
    translation.SetIdentity();
    rotation.SetIdentity();
    translation.Translate(0.0f, 0.0f, -5.0f);


    math::Vector3f blockPosition = {0.0f, 0.0f, 0.0f};
    float blockSize = 1.0f;
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


    std::vector<float> pos = {
        vertices[0].x, vertices[0].y, vertices[0].z,
        vertices[1].x, vertices[1].y, vertices[1].z,
        vertices[2].x, vertices[2].y, vertices[2].z,
        vertices[3].x, vertices[3].y, vertices[3].z,

        vertices[4].x, vertices[4].y, vertices[4].z,
        vertices[5].x, vertices[5].y, vertices[5].z,
        vertices[6].x, vertices[6].y, vertices[6].z,
        vertices[7].x, vertices[7].y, vertices[7].z
       };

    std::vector<float> tex = {
        0.0, 0.0,
        1.0, 0.0,
        0.0, 1.0,
        1.0, 1.0f
    };

    renderer::VertexBuffer modelPos(pos, 3 * sizeof(float));
    renderer::VertexBuffer modelTex(tex, 2 * sizeof(float));

    renderer::VertexFormat cubeFormat(GX_VTXFMT0);
    cubeFormat.AddAttribute({GX_INDEX16, GX_VA_POS, GX_POS_XYZ, GX_F32});
    cubeFormat.AddAttribute({GX_INDEX16, GX_VA_TEX0, GX_TEX_ST, GX_F32});

    std::shared_ptr<renderer::VertexArray> vertexArray = std::make_shared<renderer::VertexArray>(&cubeFormat);
    vertexArray->AddVertexBuffer(GX_VA_POS, &modelPos);
    vertexArray->AddVertexBuffer(GX_VA_TEX0, &modelTex);


    std::shared_ptr<renderer::IndexBuffer> indexBuffer =
            std::make_shared<renderer::IndexBuffer>(std::initializer_list<uint16_t>{
                                     0, 0,
                                     3, 1,
                                     2, 3,
                                     1, 2,

                                     5, 0,
                                     4, 1,
                                     7, 3,
                                     6, 3,

                                     3,0,
                                     5,1,
                                     6,3,
                                     2,2,

                                     4,0,
                                     0,1,
                                     1,3,
                                     7,2,

                                     4,0,
                                     5,1,
                                     3,3,
                                     0,2,

                                     6,0,
                                     7,1,
                                     1,3,
                                     2,2
                        });

    renderer::StaticMesh cube(indexBuffer, vertexArray, GX_QUADS);
    cube.SetTexture(texture);

    renderer.LoadFont(Minecraft_ttf, Minecraft_ttf_size, 64);

    while(true)
    {

        WPAD_ScanPads();
        s_wpadData = WPAD_Data(WPAD_CHAN_0);
        s_wpadButton.ButtonUp = WPAD_ButtonsUp(WPAD_CHAN_0);
        s_wpadButton.ButtonDown = WPAD_ButtonsDown(WPAD_CHAN_0);
        s_wpadButton.ButtonHeld = WPAD_ButtonsHeld(WPAD_CHAN_0);

        if (s_wpadButton.ButtonHeld & WPAD_BUTTON_HOME)
            break;

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

        renderer.SetCamera(&perspectiveCamera);       
        DrawIndexedDummy3DTexturedCube(clock, renderer, translation, rotation, cube);

        renderer.SetCamera(&orthographicCamera);        
        DrawDummySprite(cursorSprite, renderer, true);

        renderer.LoadModelViewMatrix(math::Matrix3x4::Identity());        
        renderer.DrawText(320, 275, L"Hello World", renderer::ColorRGBA::GREEN);

        renderer.DisplayBuffer();
    }
}


void DrawIndexedDummy3DTexturedCube(utils::Clock& clock, renderer::Renderer& renderer,
                                    math::Matrix3x4& translation, math::Matrix3x4& rotation,
                                    renderer::StaticMesh& mesh)
{
    /*GX_SetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
    GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GX_SetColorUpdate(GX_TRUE);*/

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
    math::Matrix3x4 modelView = viewMatrix * (translation * rotation * scale);

    renderer.LoadModelViewMatrix(modelView);
    renderer.Draw(mesh);
}



void DrawFixedDummy3DTexturedCube(utils::Clock& clock, renderer::Renderer &renderer, math::Matrix3x4& translation, math::Matrix3x4& rotation)
{

    GX_SetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
    GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GX_SetColorUpdate(GX_TRUE);

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

    GX_LoadPosMtxImm(modelView.mMtx34, GX_PNMTX0);
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
        GX_TexCoord2f32(0.0f, 0.0f);

        GX_Position3f32(vertices[3].x, vertices[3].y, vertices[3].z);
        GX_TexCoord2f32(1.0f, 0.0f);


        GX_Position3f32(vertices[2].x, vertices[2].y, vertices[2].z);
        GX_TexCoord2f32(1.0f, 1.0f);


        GX_Position3f32(vertices[1].x, vertices[1].y, vertices[1].z);
        GX_TexCoord2f32(0.0f, 1.0f);


        // back side
        GX_Position3f32(vertices[5].x, vertices[5].y, vertices[5].z);
        GX_TexCoord2f32(0.0f, 0.0f);


        GX_Position3f32(vertices[4].x, vertices[4].y, vertices[4].z);
        GX_TexCoord2f32(1.0f, 0.0f);


        GX_Position3f32(vertices[7].x, vertices[7].y, vertices[7].z);
        GX_TexCoord2f32(1.0f, 1.0f);


        GX_Position3f32(vertices[6].x, vertices[6].y, vertices[6].z);
        GX_TexCoord2f32(0.0f, 1.0f);



        // right side
        GX_Position3f32(vertices[3].x, vertices[3].y, vertices[3].z);
        GX_TexCoord2f32(0.0f, 0.0f);


        GX_Position3f32(vertices[5].x, vertices[5].y, vertices[5].z);
        GX_TexCoord2f32(1.0f, 0.0f);


        GX_Position3f32(vertices[6].x, vertices[6].y, vertices[6].z);
        GX_TexCoord2f32(1.0f, 1.0f);


        GX_Position3f32(vertices[2].x, vertices[2].y, vertices[2].z);
        GX_TexCoord2f32(0.0f, 1.0f);



        // left side
        GX_Position3f32(vertices[4].x, vertices[4].y, vertices[4].z);
        GX_TexCoord2f32(0.0f, 0.0f);


        GX_Position3f32(vertices[0].x, vertices[0].y, vertices[0].z);
        GX_TexCoord2f32(1.0f, 0.0f);


        GX_Position3f32(vertices[1].x, vertices[1].y, vertices[1].z);
        GX_TexCoord2f32(1.0f, 1.0f);


        GX_Position3f32(vertices[7].x, vertices[7].y, vertices[7].z);
        GX_TexCoord2f32(0.0f, 1.0f);



        // top side
        GX_Position3f32(vertices[4].x, vertices[4].y, vertices[4].z);
        GX_TexCoord2f32(0.0f, 0.0f);


        GX_Position3f32(vertices[5].x, vertices[5].y, vertices[5].z);
        GX_TexCoord2f32(1.0f, 0.0f);


        GX_Position3f32(vertices[3].x, vertices[3].y, vertices[3].z);
        GX_TexCoord2f32(1.0f, 1.0f);


        GX_Position3f32(vertices[0].x, vertices[0].y, vertices[0].z);
        GX_TexCoord2f32(0.0f, 1.0f);



        // bottom side
        GX_Position3f32(vertices[6].x, vertices[6].y, vertices[6].z);
        GX_TexCoord2f32(0.0f, 0.0f);


        GX_Position3f32(vertices[7].x, vertices[7].y, vertices[7].z);
        GX_TexCoord2f32(1.0f, 0.0f);


        GX_Position3f32(vertices[1].x, vertices[1].y, vertices[1].z);
        GX_TexCoord2f32(1.0f, 1.0f);


        GX_Position3f32(vertices[2].x, vertices[2].y, vertices[2].z);
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
    translation.Translate(sprite.PosX(), sprite.PosY(), sprite.PosZ());
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
