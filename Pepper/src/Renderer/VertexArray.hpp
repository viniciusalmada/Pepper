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

    virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& buffer) = 0;

    virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& buffer) = 0;

    virtual const std::set<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;

    virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;

    virtual uint32_t GetRendererID() const = 0;

    static std::shared_ptr<VertexArray> Create();
  };
}
