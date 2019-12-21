#include "tpl_loader.h"
#include "image2d.h"

bool renderer::IsTPLTexture(const uint8_t *data)
{
    if (!data)
    {
        return false;
    }
    const TPL_Header* pHeader = reinterpret_cast<const TPL_Header*>(data);
    return (pHeader && pHeader->magic == 0x20af30);
}

renderer::TPL_Texture* renderer::GetTPLTexture(const uint8_t *data)
{
    if (!data)
    {
        return nullptr;
    }
    return reinterpret_cast<TPL_Texture*>((const_cast<uint8_t*>(data) + sizeof(TPL_Header) + sizeof(TPL_Addr)));
}

size_t renderer::GetTPLTextureSize(const size_t imageSize)
{
    return imageSize - sizeof(TPL_Header) - sizeof(TPL_Addr) - sizeof(TPL_Texture);
}

renderer::TPL_Header* renderer::GetTPLHeader(uint8_t *data)
{
    if (!data)
    {
        return nullptr;
    }
    return reinterpret_cast<TPL_Header*>(data);
}
