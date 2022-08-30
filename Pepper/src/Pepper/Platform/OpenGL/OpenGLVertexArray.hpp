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

    uint32_t GetRendererID() const override;

  private:
    DECLARE_PIMPL
  };
}