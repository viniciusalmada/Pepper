#pragma once

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
    static VertexBuffer* Create(float* vertices, uint32_t size);
  };

  class IndexBuffer : public Buffer
  {
  public:
    virtual uint32_t GetCount() const = 0;

    static IndexBuffer* Create(uint32_t* indices, uint32_t count);
  };
}