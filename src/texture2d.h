#pragma once

#include <stdint.h>

namespace renderer {

class Image2D;
class Renderer;
class Texture2DData;

class Texture2D
{
    friend class Renderer;

public:
    explicit Texture2D(const Image2D& image);
    ~Texture2D();
    Texture2D(const Texture2D&) = default;
    Texture2D(Texture2D&&) = default;
    Texture2D& operator=(const Texture2D&) = default;
    Texture2D& operator=(Texture2D&&) = default;

    void Bind(uint8_t unit = 0);

private:
    Texture2DData* mTextureData;
};

}
