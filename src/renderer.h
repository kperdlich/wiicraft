#pragma once

#include <memory>
#include "colorrgba.h"
#include "renderdata.h"
#include "camera.h"

namespace renderer {

class Renderer {
public:
    explicit Renderer(bool useVSync);
    ~Renderer() = default;
    Renderer(const Renderer&) = delete;
    Renderer& operator = (const Renderer&) = delete;
    Renderer(Renderer&&) = delete;
    Renderer& operator = (Renderer&&) = delete;

    void SetClearColor(const ColorRGBA& clearColor);
    void DisplayBuffer();
    void SetCamera(std::shared_ptr<Camera> camera);

    uint32_t GetWidth() const;
    uint32_t GetHeight() const;

private:
    std::unique_ptr<RenderData> mRenderData;
    std::shared_ptr<Camera> mCamera;
};
};
