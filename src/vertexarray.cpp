#include <assert.h>
#include "wiidefines.h"
#include "renderer.h"
#include "renderdata.h"
#include "vertexarray.h"

renderer::VertexArray::VertexArray(VertexFormat *vertexFormat)
    : mVertexFormat(vertexFormat)
{
}

void renderer::VertexArray::AddVertexBuffer(uint32_t vertexAttribute, VertexBuffer* vertexBuffer)
{
    assert(vertexBuffer != nullptr);
    assert(mVertexBufferMap.find(vertexAttribute) == mVertexBufferMap.end());
    mVertexBufferMap[vertexAttribute] = vertexBuffer;
}

void renderer::VertexArray::Bind(renderer::Renderer &renderer)
{
    mVertexFormat->Bind();

    for (auto& vertexBuffer : mVertexBufferMap)
    {
        GX_SetArray(vertexBuffer.first, vertexBuffer.second->GetBuffer(), vertexBuffer.second->GetStride());
    }

    renderer.GetRenderData()->SetVertexArray(this);
}


