// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "VertexArray.hpp"

#include "Platform/OpenGL/OpenGLVertexArray.hpp"
#include "Renderer.hpp"

std::shared_ptr<Pepper::VertexArray> Pepper::VertexArray::Create()
{
  switch (Renderer::GetAPI())
  {
  case RendererAPI::OPEN_GL:
    return std::make_shared<OpenGLVertexArray>();
  default:
    PP_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
    return nullptr;
  }
}