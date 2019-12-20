#pragma once

#include <array>
#include <memory>
#include "matrix3x4.h"
#include "colorrgba.h"
#include "statistics.h"

namespace core {
    class AABB;
}

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

std::array<float, 8> GetUVTextureCoordinates(uint32_t index, uint32_t width, uint32_t height, uint32_t tileWidth, uint32_t tileHeight);

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
    void SetCamera(std::shared_ptr<Camera> camera);

    void SetZModeEnabled(bool isEnabled);
    void SetCullMode(const CullMode& mode);
    void EnableFog(const float startZ, const float endZ, const ColorRGBA &color);
    void DisableFog();

    void LoadModelViewMatrix(const math::Matrix3x4& modelView, const uint8_t matrixIndex = 0);
    void LoadFont(const uint8_t* fontData, const int32_t size, const uint32_t fontSize);
    void SetLineWidth(uint8_t width);

    void DrawText(int32_t x, int32_t y, const std::wstring& text, const ColorRGBA &color);
    void DrawSpriteSheet(int32_t x, int32_t y, Sprite &sprite, uint32_t index,  uint32_t tileWidth, uint32_t tileHeight, uint32_t finalSpriteWidth, uint32_t finalSpriteHeight);
    void DrawSpriteSheet(int32_t x, int32_t y, Sprite &sprite, uint32_t tileX, uint32_t tileY, uint32_t tileWidth, uint32_t tileHeight, uint32_t finalSpriteWidth, uint32_t finalSpriteHeight);
    void Draw(Mesh& mesh);
    void Draw(Sprite &sprite);
    void Draw(StaticMesh& mesh);
    void DrawLine(const math::Vector3f &from, const math::Vector3f& end, const renderer::ColorRGBA& color);
    void DrawRay(const math::Vector3f &from, const math::Vector3f &direction, const renderer::ColorRGBA& color);
    void DrawAABB(const core::AABB& aabb, const renderer::ColorRGBA& color);

    void ClearStatistics();
    void UpdateFPS();

    uint32_t GetWidth() const;
    uint32_t GetHeight() const;

    inline std::shared_ptr<renderer::Camera> GetCamera() const;
    inline RenderData* GetRenderData();
    inline Statistics& GetStatistics();
    inline const Statistics& GetStatistics() const;

    static Renderer* s_Renderer;
private:
    Statistics mStatistics;
    RenderData* mRenderData;    
    std::shared_ptr<renderer::Camera> mCamera;
};

inline std::shared_ptr<Camera> Renderer::GetCamera() const
{
    return mCamera;
}

inline RenderData* Renderer::GetRenderData()
{
    return mRenderData;
}

inline Statistics &Renderer::GetStatistics()
{
    return mStatistics;
}

inline const Statistics &Renderer::GetStatistics() const
{
    return mStatistics;
}

};
