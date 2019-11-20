#pragma once

#include <memory>
#include "core.h"
#include <map>
#include "vertexbuffer.h"
#include "vertexformat.h"
#include "indexbuffer.h"

namespace renderer {

using VertexBufferMap = std::map<uint32_t, VertexBuffer*>;

class Renderer;

class VertexArray
{
    friend class Renderer;

public:
    VertexArray() = default;
    explicit VertexArray(VertexFormat* vertexFormat);
    ~VertexArray() = default;
    VertexArray(const VertexArray&) = delete;
    VertexArray(VertexBuffer&&) = delete;
    VertexArray& operator=(const VertexArray&) = delete;
    VertexArray& operator=(VertexArray&&) = delete;

    void AddVertexBuffer(uint32_t vertexAttribute, VertexBuffer *vertexBuffer);

    inline void SetVertexFormat(VertexFormat *vertexFormat);
    inline uint8_t GetVertexFormatIndex() const;

    inline const VertexBufferMap& GetVertexBufferMap() const;

private:
    void Bind();

    VertexFormat* mVertexFormat;
    std::map<uint32_t, VertexBuffer*> mVertexBufferMap;
};

inline void VertexArray::SetVertexFormat(VertexFormat* vertexFormat)
{
    mVertexFormat = vertexFormat;
}

inline uint8_t VertexArray::GetVertexFormatIndex() const
{
    ASSERT(mVertexFormat != nullptr);
    return mVertexFormat->GetFormatIndex();
}

inline const VertexBufferMap &VertexArray::GetVertexBufferMap() const
{
    return mVertexBufferMap;
}

}
