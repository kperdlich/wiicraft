#pragma once

#include "wiidefines.h"

namespace renderer {

class Image2D;
class Renderer;

class Texture2D
{
public:
    explicit Texture2D(const Image2D& image);
    ~Texture2D() = default;
    Texture2D(const Texture2D&) = default;
    Texture2D(Texture2D&&) = default;
    Texture2D& operator=(const Texture2D&) = default;
    Texture2D& operator=(Texture2D&&) = default;

    void Bind(Renderer& renderer, uint8_t unit);

private:
    GXTexObj mTexObj;
};

}
