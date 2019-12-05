#pragma once

#include "vector"
#include "stdint.h"
#include <stddef.h>

namespace renderer {

struct VertexFormatAttribute
{
    uint8_t DataInputType;
    uint8_t Attribute;
    uint8_t ComponentType;
    uint8_t ComponentSizeType;
};

class VertexFormat {
    friend class VertexArray;
    friend class Renderer;
public:
    VertexFormat() = default;
    explicit VertexFormat(uint32_t formatIndex);
    ~VertexFormat() = default;
    VertexFormat(const VertexFormat&) = delete;
    VertexFormat(VertexFormat&&) = delete;
    VertexFormat& operator=(const VertexFormat&) = delete;
    VertexFormat& operator=(VertexFormat&&) = delete;

    void AddAttribute(const VertexFormatAttribute& attribute);

    inline size_t VertexAttributeCount() const;
    inline uint8_t GetFormatIndex() const;
    inline void SetFormatIndex(uint8_t format);

    void Bind();

private:
    std::vector<VertexFormatAttribute> mAttributes;
    uint8_t mFormatIndex;
};

inline size_t VertexFormat::VertexAttributeCount() const
{
    return mAttributes.size();
}

inline uint8_t VertexFormat::GetFormatIndex() const
{
    return mFormatIndex;
}

inline void VertexFormat::SetFormatIndex(uint8_t format)
{
    mFormatIndex = format;
}

}
