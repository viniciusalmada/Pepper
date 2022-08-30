// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Pepper/Renderer/VertexArray.hpp"

#include "Pepper/Platform/OpenGL/OpenGLVertexArray.hpp"
#include "Pepper/Renderer/Renderer.hpp"

namespace Pepper
{
  Ref<VertexArray> VertexArray::Create()
  {
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::OPEN_GL:
      return std::make_shared<OpenGLVertexArray>();
    default:
      PP_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
      return nullptr;
    }
  }
}