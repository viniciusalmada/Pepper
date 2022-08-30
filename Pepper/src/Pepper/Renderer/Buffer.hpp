#pragma once

#include "BufferElement.hpp"
#include "BufferLayout.hpp"
#include "Pepper/Core/Core.hpp"

#include <cstdint>

namespace Pepper
{
  class Buffer
  {
  public:
    virtual ~Buffer() = default;

    virtual void Bind() const = 0;

    virtual void Unbind() const = 0;
  };

  class VertexBuffer : public Buffer
  {
  public:
    virtual const BufferLayout& GetLayout() const = 0;

    virtual void SetLayout(const BufferLayout& layout) = 0;

    static Ref<VertexBuffer> Create(float* vertices, uint32_t size, uint32_t parent);
  };

  class IndexBuffer : public Buffer
  {
  public:
    virtual uint32_t GetCount() const = 0;

    static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count, uint32_t parent);
  };
}