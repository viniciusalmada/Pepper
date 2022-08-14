// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Buffer.hpp"

#include "Platform/OpenGL/OpenGLBuffer.hpp"
#include "Renderer.hpp"

Pepper::VertexBuffer* Pepper::VertexBuffer::Create(float* vertices,
                                                   uint32_t size)
{
  switch (Renderer::GetAPI())
  {
  case RendererAPI::OPEN_GL:
    return new OpenGLVertexBuffer(vertices, size);
  default:
    PP_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
    return nullptr;
  }
}

Pepper::IndexBuffer* Pepper::IndexBuffer::Create(uint32_t* indices,
                                                 uint32_t count)
{
  switch (Renderer::GetAPI())
  {
  case RendererAPI::OPEN_GL:
    return new OpenGLIndexBuffer(indices, count);
  default:
    PP_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
    return nullptr;
  }
}
