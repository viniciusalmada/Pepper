#pragma once

#include "Buffer.hpp"

#include <memory>
#include <set>

namespace Pepper
{
  class VertexArray
  {
  public:
    virtual ~VertexArray() = default;

    virtual void Bind() const = 0;

    virtual void Unbind() const = 0;

    virtual void AddVertexBuffer(const Ref<VertexBuffer>& buffer) = 0;

    virtual void SetIndexBuffer(const Ref<IndexBuffer>& buffer) = 0;

    virtual const std::set<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;

    virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

    virtual uint32_t GetRendererID() const = 0;

    static VertexArray* Create();
  };
}
