#pragma once

#include "Pepper/Renderer/Buffer.hpp"

#include <cstdint>

namespace Pepper
{
  class OpenGLVertexBuffer : public VertexBuffer
  {
  public:
    OpenGLVertexBuffer(float* vertices, uint32_t size, uint32_t parent);

    ~OpenGLVertexBuffer() override;

    void Bind() const override;

    void Unbind() const override;

    const BufferLayout& GetLayout() const override;

    void SetLayout(const BufferLayout& layout) override;

  private:
    DECLARE_PIMPL
  };

  class OpenGLIndexBuffer : public IndexBuffer
  {
  public:
    OpenGLIndexBuffer(uint32_t* indices, uint32_t count, uint32_t parent);

    ~OpenGLIndexBuffer() override;

    void Bind() const override;

    void Unbind() const override;

    uint32_t GetCount() const override;

  private:
    DECLARE_PIMPL
  };
}
