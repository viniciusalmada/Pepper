// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Buffer.hpp"

#include "Platform/OpenGL/OpenGLBuffer.hpp"
#include "Renderer.hpp"

std::shared_ptr<Pepper::VertexBuffer> Pepper::VertexBuffer::Create(float* vertices, uint32_t size, uint32_t parent)
{
  switch (Renderer::GetAPI())
  {
  case RendererAPI::OPEN_GL:
    return std::make_shared<OpenGLVertexBuffer>(vertices, size, parent);
  default:
    PP_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
    return nullptr;
  }
}

std::shared_ptr<Pepper::IndexBuffer> Pepper::IndexBuffer::Create(uint32_t* indices, uint32_t count, uint32_t parent)
{
  switch (Renderer::GetAPI())
  {
  case RendererAPI::OPEN_GL:
    return std::make_shared<OpenGLIndexBuffer>(indices, count, parent);
  default:
    PP_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
    return nullptr;
  }
}
