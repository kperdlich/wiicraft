#pragma once

#include <memory>
#include "wii_displaylist.h"
#include "matrix3x4.h"
#include "texture2d.h"
#include "image2d.h"

namespace renderer {

class Camera;

class SkyBox {
public:
    SkyBox();
    ~SkyBox() = default;
	SkyBox(const SkyBox&) = delete;
	SkyBox(SkyBox&&) = delete;
	void operator=(const SkyBox&) = delete;
	void operator=(SkyBox&&) = delete;
    void Render(renderer::Renderer& renderer);
private:
    void CreateSkyBox(renderer::Renderer& renderer);
    std::unique_ptr<renderer::Texture2D> m_pSkyBoxTextures[6];
    std::unique_ptr<renderer::Image2D> mImages[6];
    renderer::DisplayList m_displayList;
};
}
