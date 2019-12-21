#pragma once

#include "stdint.h"

namespace renderer {
class ColorRGBA
{
public:
    ColorRGBA();
    ColorRGBA(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
    ~ColorRGBA() = default;
    ColorRGBA(const ColorRGBA&) = default;
    ColorRGBA& operator = (const ColorRGBA&) = default;
    ColorRGBA(ColorRGBA&&) = default;
    ColorRGBA& operator = (ColorRGBA&&) = default;

    inline uint8_t Red() const;
    inline uint8_t Green() const;
    inline uint8_t Blue() const;
    inline uint8_t Alpha() const;
    inline uint32_t Color() const;

    static const ColorRGBA RED;
    static const ColorRGBA GREEN;
    static const ColorRGBA BLUE;
    static const ColorRGBA BLACK;
    static const ColorRGBA WHITE;
    static const ColorRGBA YELLOW;
    static const ColorRGBA ORANGE;

private:
    uint8_t mChannels[4];
};

inline uint8_t ColorRGBA::Red() const
{
    return mChannels[0];
}

inline uint8_t ColorRGBA::Green() const
{
    return mChannels[1];
}

inline uint8_t ColorRGBA::Blue() const
{
    return mChannels[2];
}

inline uint8_t ColorRGBA::Alpha() const
{
    return mChannels[3];

}

inline uint32_t ColorRGBA::Color() const
{
    return static_cast<uint32_t>(((mChannels[0] & 0xff) << 24) + ((mChannels[1] & 0xff) << 16) + ((mChannels[2] & 0xff) << 8) + (mChannels[3] & 0xff));
}

}


