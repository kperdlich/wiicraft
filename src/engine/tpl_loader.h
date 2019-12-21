#pragma once

#include <stdint.h>
#include <stddef.h>

namespace renderer {

class Image2D;

struct TPL_Header
{
    uint32_t	magic;
    uint32_t	texCount;
    uint32_t	headerSize;
};

struct TPL_Addr
{
    uint32_t	textureOffs;
    uint32_t	tlutOffs;
};

struct TPL_Texture
{
    uint16_t	height;
    uint16_t	width;
    uint32_t	format;
    uint32_t	dataOffs;
    uint32_t	wrap_s;
    uint32_t	wrap_t;
    uint32_t	minFilt;
    uint32_t	magFilt;
    float       lodBias;
    uint8_t     edgeLod;
    uint8_t     minLod;
    uint8_t     maxLod;
    uint8_t     unpacked;
};


TPL_Header* GetTPLHeader(uint8_t *data);
TPL_Texture* GetTPLTexture(const uint8_t *data);
bool IsTPLTexture(const uint8_t *data);
size_t GetTPLTextureSize(const size_t imageSize);


}
