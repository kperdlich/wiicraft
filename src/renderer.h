#pragma once

#include <memory>
#include "matrix3x4.h"
#include "colorrgba.h"

namespace renderer {

enum class CullMode : uint8_t {
    None = 0,
    Front = 1,
    Back = 2,
    All = 3
};

class RenderData;
class TTFFont;
class Mesh;
class StaticMesh;
class Sprite;
class Camera;

class Renderer {
public:
    explicit Renderer(bool useVSync);
    ~Renderer();
    Renderer(const Renderer&) = delete;
    Renderer& operator = (const Renderer&) = delete;
    Renderer(Renderer&&) = delete;
    Renderer& operator = (Renderer&&) = delete;

    void SetClearColor(const ColorRGBA& clearColor);
    void PreDraw();
    void DisplayBuffer();
    void SetCamera(Camera *camera);

    void SetZModeEnabled(bool isEnabled);
    void SetCullMode(const CullMode& mode);

    void LoadModelViewMatrix(const math::Matrix3x4& modelView, const uint8_t matrixIndex = 0);
    void LoadFont(const uint8_t* fontData, const int32_t size, const uint32_t fontSize);

    void SetLineWidth(uint8_t width);

    void DrawText(int32_t x, int32_t y, const std::wstring& text, const ColorRGBA &color);
    void Draw(Mesh& mesh);
    void Draw(Sprite &sprite);
    void Draw(StaticMesh& mesh);
    void DrawLine(const math::Vector3f &from, const math::Vector3f& end, const renderer::ColorRGBA& color);
    void DrawRay(const math::Vector3f &from, const math::Vector3f &direction, const renderer::ColorRGBA& color);

    uint32_t GetWidth() const;
    uint32_t GetHeight() const;

    inline Camera* GetCamera() const;
    inline RenderData* GetRenderData();

    static Renderer* s_Renderer;
private:
    RenderData* mRenderData;
    Camera* mCamera;
};

inline Camera *Renderer::GetCamera() const
{
    return mCamera;
}

inline RenderData* Renderer::GetRenderData()
{
    return mRenderData;
}

};
