#pragma once

#include <memory>
#include "indexbuffer.h"
#include "vertexbuffer.h"

namespace renderer {

class Mesh
{
public:
    Mesh(std::shared_ptr<IndexBuffer> indexBuffer, std::shared_ptr<VertexBuffer> vertexBuffer);
    ~Mesh() = default;
    Mesh(const Mesh&) = delete;
    Mesh& operator = (const Mesh&) = delete;
    Mesh(Mesh&&) = delete;
    Mesh& operator = (Mesh&&) = delete;
private:
    std::shared_ptr<VertexBuffer> mVertexBuffer;
    std::shared_ptr<IndexBuffer> mIndexBuffer;
};

}
