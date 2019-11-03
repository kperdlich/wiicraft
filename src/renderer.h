#pragma once

#include <memory>
#include "colorrgba.h"
#include "renderdata.h"

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

private:
    std::unique_ptr<RenderData> mRenderData;
};
};
