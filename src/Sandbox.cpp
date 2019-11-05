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

int main(int argc, char** argv)
{    
    renderer::Renderer renderer(true);
    std::shared_ptr<renderer::Camera> camera =
            std::shared_ptr<renderer::Camera>(new renderer::Camera({.0f, .0f, .1f}, {.0f, 1.0f, .0f}, {.0f, .0f, .0f}, false));
    camera->SetFrustrum(.1f, 200.0f, 70.0f, renderer.GetWidth() / renderer.GetHeight());
    renderer.SetCamera(camera);
    utils::Clock clock;
    clock.Start();
    const renderer::ColorRGBA* color = &renderer::ColorRGBA::RED;
    while(true)
    {        
        if (clock.ElapsedMilliseconds() >= 1000 * 2)
        {
            if (color == &renderer::ColorRGBA::RED)
            {
                color = &renderer::ColorRGBA::BLUE;
            }
            else
            {
                color = &renderer::ColorRGBA::RED;
            }
            renderer.SetClearColor(*color);
            clock.Start();
        }
        renderer.DisplayBuffer();
    }
}
