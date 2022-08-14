#pragma once

#include "Renderer/VertexArray.hpp"

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

    void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& buffer) override;

    void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& buffer) override;

    const std::set<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override;

    const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override;

    uint32_t GetRendererID() const override { return renderer_id; };

  private:
    uint32_t renderer_id;
    std::set<std::shared_ptr<VertexBuffer>> vertex_buffers;
    std::shared_ptr<IndexBuffer> index_buffer;
  };
}