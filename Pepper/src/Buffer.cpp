// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Pepper/Renderer/Buffer.hpp"

#include "Pepper/Platform/OpenGL/OpenGLBuffer.hpp"
#include "Pepper/Renderer/Renderer.hpp"

namespace Pepper
{

  Ref<VertexBuffer> VertexBuffer::Create(const std::vector<float>& vertices, uint32_t parent)
  {
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::OPEN_GL:
      return std::make_shared<OpenGLVertexBuffer>(vertices, parent);
    case RendererAPI::API::NONE:
      PP_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
      return nullptr;
    }
    PP_CORE_ASSERT(false, "Unreachable code");
    return nullptr;
  }

  Ref<IndexBuffer> IndexBuffer::Create(const std::vector<uint32_t>& indices, uint32_t parent)
  {
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::OPEN_GL:
      return std::make_shared<OpenGLIndexBuffer>(indices, parent);
    case RendererAPI::API::NONE:
      PP_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
      return nullptr;
    }
    PP_CORE_ASSERT(false, "Unreachable code");
    return nullptr;
  }
}