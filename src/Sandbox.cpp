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
#include "wiisprite.h"
#include "ClassicBackgroundSprite_png.h"
#include "Cursor_png.h"
#include <assert.h>

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
    std::shared_ptr<renderer::Camera> camera =
            std::make_shared<renderer::Camera>(math::Vector3f{.0f, .0f, .1f}, math::Vector3f{.0f, 1.0f, .0f}, math::Vector3f{.0f, .0f, .0f}, false);
    camera->SetFrustrum(0, renderer.GetHeight(), 0, renderer.GetWidth(), 0, 100.0f);
    renderer.SetCamera(camera);
    renderer.SetZModeEnabled(true);
    renderer.SetClearColor(renderer::ColorRGBA::RED);
    renderer::VertexFormat vertexFormat(GX_VTXFMT0);
    vertexFormat.AddAttribute({GX_DIRECT, GX_VA_POS, GX_POS_XYZ, GX_F32});
    //vertexFormat.AddAttribute({GX_DIRECT, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8});
    vertexFormat.AddAttribute({GX_DIRECT, GX_VA_TEX0, GX_TEX_ST, GX_F32});

    vertexFormat.Bind();
    utils::Clock clock;
    clock.Start();

    WPAD_Init();
    WPAD_SetVRes(WPAD_CHAN_0, renderer.GetWidth(), renderer.GetHeight());

    renderer::Image2D backgroundImage(ClassicBackgroundSprite_png);
    renderer::Image2D cursorImage(Cursor_png);
    renderer::Sprite cursorSprite(cursorImage);
    cursorSprite.SetPosX(100.0f);
    cursorSprite.SetPosY(100.0f);
    cursorSprite.SetRotationDeg(180.0f);
    cursorSprite.SetScaleX(0.6f);
    renderer::Sprite backgroundSprite(backgroundImage);
    backgroundSprite.SetPosX(renderer.GetWidth() * 0.5f);
    backgroundSprite.SetPosY(renderer.GetHeight() * 0.5f);
    while(true)
    {        

        WPAD_ScanPads();
        s_wpadData = WPAD_Data(WPAD_CHAN_0);
        s_wpadButton.ButtonUp = WPAD_ButtonsUp(WPAD_CHAN_0);
        s_wpadButton.ButtonDown = WPAD_ButtonsDown(WPAD_CHAN_0);
        s_wpadButton.ButtonHeld = WPAD_ButtonsHeld(WPAD_CHAN_0);
        //DrawDummyColorTriangle(clock);
        backgroundSprite.Bind(renderer, 0);
        DrawDummySprite(backgroundSprite, renderer, false);
        cursorSprite.Bind(renderer, 0);
        DrawDummySprite(cursorSprite, renderer, true);
        renderer.DisplayBuffer();
    }
}

void DrawDummySprite(renderer::Sprite& sprite, renderer::Renderer& renderer, bool cursor)
{
    if (cursor)
    {
        if (s_wpadButton.ButtonHeld & WPAD_BUTTON_LEFT)
            sprite.SetPosX(sprite.PosX() - .1f);
        if (s_wpadButton.ButtonHeld & WPAD_BUTTON_RIGHT)
            sprite.SetPosX(sprite.PosX() + .1f);
        if (s_wpadButton.ButtonHeld & WPAD_BUTTON_UP)
            sprite.SetPosY(sprite.PosY() - .1f);
        if (s_wpadButton.ButtonHeld & WPAD_BUTTON_DOWN)
            sprite.SetPosY(sprite.PosY() + .1f);
    }
    math::Matrix3x4 viewMatrix = renderer.GetCamera()->GetViewMatrix3x4();

    GX_SetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
    GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GX_SetColorUpdate(GX_TRUE);


    math::Matrix3x4 translation, scale, rotation, model, modelView;
    translation.SetIdentity();
    scale.SetIdentity();
    rotation.SetIdentity();
    model.SetIdentity();
    modelView.SetIdentity();

    scale.Scale(sprite.ScaleX(), sprite.ScaleY(), 0.0f);
    translation.Translate(sprite.PosX(), sprite.PosY(), 0);
    rotation.Rotate('Z', sprite.RotationDeg());

    model = translation * rotation * scale;
    modelView = viewMatrix * model;

    float width = sprite.Width() * .5f;
    float height = sprite.Height() * .5f;

    GX_LoadPosMtxImm(modelView.mMtx34, GX_PNMTX0);
    GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
        GX_Position3f32(-width, -height, 0);
        GX_TexCoord2f32(0, 0);

        GX_Position3f32(width, -height, 0);
        GX_TexCoord2f32(1, 0);

        GX_Position3f32(width, height, 0);
        GX_TexCoord2f32(1, 1);

        GX_Position3f32(-width, height, 0);
        GX_TexCoord2f32(0, 1);
    GX_End();

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
