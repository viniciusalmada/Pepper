#pragma once

#include "Pepper/Renderer/VertexArray.hpp"

#include <cstdint>
#include <memory>
#include <set>

namespace Pepper
{
  class OpenGLVertexArray : public VertexArray
  {
  public:
    OpenGLVertexArray();

    ~OpenGLVertexArray() override;

    void Bind() const override;

    void Unbind() const override;

    void AddVertexBuffer(const Ref<VertexBuffer>& buffer) override;

    void SetIndexBuffer(const Ref<IndexBuffer>& buffer) override;

    const std::set<Ref<VertexBuffer>>& GetVertexBuffers() const override;

    const Ref<IndexBuffer>& GetIndexBuffer() const override;

    uint32_t GetRendererID() const override { return renderer_id; };

  private:
    uint32_t renderer_id;
    std::set<Ref<VertexBuffer>> vertex_buffers;
    Ref<IndexBuffer> index_buffer;
  };
}