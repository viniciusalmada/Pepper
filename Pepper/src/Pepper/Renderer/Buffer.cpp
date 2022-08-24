// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Buffer.hpp"

#include "Pepper/Platform/OpenGL/OpenGLBuffer.hpp"
#include "Renderer.hpp"

namespace Pepper
{

  Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size, uint32_t parent)
  {
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::OPEN_GL:
      return std::make_shared<OpenGLVertexBuffer>(vertices, size, parent);
    default:
      PP_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
      return nullptr;
    }
  }

  Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count, uint32_t parent)
  {
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::OPEN_GL:
      return std::make_shared<OpenGLIndexBuffer>(indices, count, parent);
    default:
      PP_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
      return nullptr;
    }
  }
}