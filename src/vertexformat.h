#pragma once

#include "vector"
#include "stdint.h"

namespace renderer {

struct VertexFormatAttribute {
    uint8_t DataInputType;
    uint8_t Attribute;
    uint8_t ComponentType;
    uint8_t ComponentSizeType;
};

class VertexFormat {
public:
    explicit VertexFormat(uint32_t formatIndex);
    ~VertexFormat() = default;
    VertexFormat(const VertexFormat&) = delete;
    VertexFormat(VertexFormat&&) = delete;
    VertexFormat& operator=(const VertexFormat&) = delete;
    VertexFormat& operator=(VertexFormat&&) = delete;

    void Bind();
    void AddAttribute(const VertexFormatAttribute& attribute);

private:
    std::vector<VertexFormatAttribute> mAttributes;
    uint32_t mFormatIndex;
};

}
