#include "colorrgba.h"

const renderer::ColorRGBA renderer::ColorRGBA::WHITE = {255, 255, 255, 0};
const renderer::ColorRGBA renderer::ColorRGBA::BLACK = {0, 0, 0, 0};
const renderer::ColorRGBA renderer::ColorRGBA::RED = {255, 0, 0, 0};
const renderer::ColorRGBA renderer::ColorRGBA::GREEN = {0, 255, 0, 0};
const renderer::ColorRGBA renderer::ColorRGBA::BLUE = {0, 0, 255, 0};

renderer::ColorRGBA::ColorRGBA()
{
    mChannels[0] = 0;
    mChannels[1] = 0;
    mChannels[2] = 0;
    mChannels[3] = 0;
}

renderer::ColorRGBA::ColorRGBA(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
{
    mChannels[0] = red;
    mChannels[1] = green;
    mChannels[2] = blue;
    mChannels[3] = alpha;
}
