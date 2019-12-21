#pragma once

#include <memory>
#include "vertexarray.h"

namespace renderer {

class IndexBuffer;
class VertexBuffer;
class Texture2D;
class Renderer;

class Mesh
{
public:
    Mesh(std::shared_ptr<IndexBuffer> indexBuffer, std::shared_ptr<VertexArray> vertexArray, uint8_t primitiveType);
    ~Mesh() = default;
    Mesh(const Mesh&) = delete;
    Mesh& operator = (const Mesh&) = delete;
    Mesh(Mesh&&) = delete;
    Mesh& operator = (Mesh&&) = delete;    

    inline void SetTexture(std::shared_ptr<Texture2D> texture);
    inline std::shared_ptr<IndexBuffer> GetIndexBuffer();
    inline std::shared_ptr<VertexArray> GetVertexArray();
    inline std::shared_ptr<Texture2D> GetTexture();

    inline std::shared_ptr<const IndexBuffer> GetIndexBuffer() const;
    inline std::shared_ptr<const VertexArray> GetVertexArray() const;
    inline std::shared_ptr<const Texture2D> GetTexture() const;

    inline bool HasTexture() const;

    inline uint8_t GetPrimitiveType() const;
    inline uint8_t GetVertexFormatIndex() const;

private:
    std::shared_ptr<VertexArray> mVertexArray;
    std::shared_ptr<IndexBuffer> mIndexBuffer;
    std::shared_ptr<Texture2D> mTexture;
    uint8_t mPrimitiveType;
};

inline void Mesh::SetTexture(std::shared_ptr<renderer::Texture2D> texture)
{
    mTexture = texture;
}

inline std::shared_ptr<IndexBuffer> Mesh::GetIndexBuffer()
{
    return mIndexBuffer;
}

inline std::shared_ptr<VertexArray> Mesh::GetVertexArray()
{
    return mVertexArray;
}

inline std::shared_ptr<Texture2D> Mesh::GetTexture()
{
    return mTexture;
}

inline std::shared_ptr<const IndexBuffer> Mesh::GetIndexBuffer() const
{
    return mIndexBuffer;
}

inline std::shared_ptr<const VertexArray> Mesh::GetVertexArray() const
{
    return mVertexArray;
}

inline std::shared_ptr<const Texture2D> Mesh::GetTexture() const
{
    return mTexture;
}

inline bool Mesh::HasTexture() const
{
    return mTexture != nullptr;
}

inline uint8_t Mesh::GetPrimitiveType() const
{
    return mPrimitiveType;
}

inline uint8_t Mesh::GetVertexFormatIndex() const
{
    return mVertexArray->GetVertexFormatIndex();
}

}
