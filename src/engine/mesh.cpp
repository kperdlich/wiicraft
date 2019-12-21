#include "mesh.h"

renderer::Mesh::Mesh(std::shared_ptr<IndexBuffer> indexBuffer, std::shared_ptr<VertexArray> vertexArray, uint8_t primitiveType)
    : mVertexArray(vertexArray),
      mIndexBuffer(indexBuffer),
      mTexture(nullptr),
      mPrimitiveType(primitiveType) {}

