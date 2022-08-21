// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "VertexArray.hpp"

#include "Pepper/Platform/OpenGL/OpenGLVertexArray.hpp"
#include "Renderer.hpp"

Pepper::Ref<Pepper::VertexArray> Pepper::VertexArray::Create()
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