#pragma once

namespace renderer {
class ColorRGBA
{
public:
    ColorRGBA();
    ColorRGBA(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
    ~ColorRGBA() = default;
    ColorRGBA(const ColorRGBA&) = default;
    ColorRGBA& operator = (const ColorRGBA&) = default;
    ColorRGBA(ColorRGBA&&) = default;
    ColorRGBA& operator = (ColorRGBA&&) = default;

    inline unsigned char Red() const;
    inline unsigned char Green() const;
    inline unsigned char Blue() const;
    inline unsigned char Alpha() const;

    static const ColorRGBA RED;
    static const ColorRGBA GREEN;
    static const ColorRGBA BLUE;
    static const ColorRGBA BLACK;
    static const ColorRGBA WHITE;

private:
    unsigned char mChannels[4];
};

inline unsigned char ColorRGBA::Red() const
{
    return mChannels[0];
}

inline unsigned char ColorRGBA::Green() const
{
    return mChannels[1];
}

inline unsigned char ColorRGBA::Blue() const
{
    return mChannels[2];
}

inline unsigned char ColorRGBA::Alpha() const
{
    return mChannels[3];

}
}


