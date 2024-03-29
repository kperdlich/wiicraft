#include "StaticMesh.h"
#include "WiiDisplayList.h"

renderer::StaticMesh::StaticMesh(std::shared_ptr<IndexBuffer> indexBuffer, std::shared_ptr<VertexArray> vertexArray, uint8_t primitiveType)
    : mIsDirty(true)
{
    mMesh = std::make_unique<Mesh>(indexBuffer, vertexArray, primitiveType);
    mDisplayList = std::make_unique<DisplayList>();
}
