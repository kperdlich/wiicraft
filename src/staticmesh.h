#pragma once

#include <memory>
#include "mesh.h"

namespace renderer {

class IndexBuffer;
class VertexBuffer;
class VertexArray;
class DisplayList;
class Texture2D;

class StaticMesh
{
    friend class Renderer;
public:
    StaticMesh(std::shared_ptr<IndexBuffer> indexBuffer, std::shared_ptr<VertexArray> vertexArray, uint8_t primitiveType);
    ~StaticMesh() = default;
    StaticMesh(const StaticMesh&) = delete;
    StaticMesh& operator = (const StaticMesh&) = delete;
    StaticMesh(StaticMesh&&) = delete;
    StaticMesh& operator = (StaticMesh&&) = delete;

    inline void SetTexture(std::shared_ptr<Texture2D> texture);
    inline bool IsDirty() const;
private:
    std::unique_ptr<Mesh> mMesh;
    std::unique_ptr<DisplayList> mDisplayList;
    bool mIsDirty;
};

inline void StaticMesh::SetTexture(std::shared_ptr<Texture2D> texture)
{
    mMesh->SetTexture(texture);
}

inline bool StaticMesh::IsDirty() const
{
    return mIsDirty;
}

}
