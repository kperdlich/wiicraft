#include "VertexArray.h"
#include "Core.h"
#include "Renderdata.h"
#include "Renderer.h"
#include "WiiDefines.h"

renderer::VertexArray::VertexArray(VertexFormat* vertexFormat)
    : mVertexFormat(vertexFormat)
{
}

void renderer::VertexArray::AddVertexBuffer(uint32_t vertexAttribute, VertexBuffer* vertexBuffer)
{
    ASSERT(vertexBuffer != nullptr);
    ASSERT(mVertexBufferMap.find(vertexAttribute) == mVertexBufferMap.end());
    mVertexBufferMap[vertexAttribute] = vertexBuffer;
}

void renderer::VertexArray::Bind()
{
    mVertexFormat->Bind();

    for (auto& vertexBuffer : mVertexBufferMap)
    {
        GX_SetArray(vertexBuffer.first, vertexBuffer.second->GetBuffer(), vertexBuffer.second->GetStride());
    }
}
