// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "VertexArray.hpp"

#include "Pepper/Platform/OpenGL/OpenGLVertexArray.hpp"
#include "Renderer.hpp"

Pepper::VertexArray* Pepper::VertexArray::Create()
{
  switch (Renderer::GetAPI())
  {
  case RendererAPI::API::OPEN_GL:
    return new OpenGLVertexArray{};
  default:
    PP_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
    return nullptr;
  }
}