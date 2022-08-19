// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Buffer.hpp"

#include "Pepper/Platform/OpenGL/OpenGLBuffer.hpp"
#include "Renderer.hpp"

Pepper::VertexBuffer* Pepper::VertexBuffer::Create(float* vertices, uint32_t size, uint32_t parent)
{
  switch (Renderer::GetAPI())
  {
  case RendererAPI::API::OPEN_GL:
    return new OpenGLVertexBuffer{ vertices, size, parent };
  default:
    PP_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
    return nullptr;
  }
}

Pepper::IndexBuffer* Pepper::IndexBuffer::Create(uint32_t* indices, uint32_t count, uint32_t parent)
{
  switch (Renderer::GetAPI())
  {
  case RendererAPI::API::OPEN_GL:
    return new OpenGLIndexBuffer{ indices, count, parent };
  default:
    PP_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
    return nullptr;
  }
}
