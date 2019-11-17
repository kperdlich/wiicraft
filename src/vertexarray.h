#pragma once

#include <memory>
#include <unordered_map>
#include "vertexbuffer.h"
#include "vertexformat.h"
#include "indexbuffer.h"

namespace renderer {

class Renderer;

class VertexArray
{
public:
    VertexArray() = default;
    explicit VertexArray(VertexFormat* vertexFormat);
    ~VertexArray() = default;
    VertexArray(const VertexArray&) = delete;
    VertexArray(VertexBuffer&&) = delete;
    VertexArray& operator=(const VertexArray&) = delete;
    VertexArray& operator=(VertexArray&&) = delete;

    void AddVertexBuffer(uint32_t vertexAttribute, VertexBuffer *vertexBuffer);
    void Bind(Renderer &renderer);

    inline void SetVertexFormat(VertexFormat *vertexFormat);

private:
    VertexFormat* mVertexFormat;
    std::unordered_map<uint32_t, VertexBuffer*> mVertexBufferMap;
};

inline void VertexArray::SetVertexFormat(VertexFormat* vertexFormat)
{
    mVertexFormat = vertexFormat;
}

}
