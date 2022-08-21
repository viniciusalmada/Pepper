// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Shader.hpp"

#include "Pepper/Platform/OpenGL/OpenGLShader.hpp"
#include "Renderer.hpp"

Pepper::Ref<Pepper::Shader> Pepper::Shader::Create(const std::string& filepath)
{
  switch (Renderer::GetAPI())
  {
  case RendererAPI::API::OPEN_GL:
    return std::make_shared<OpenGLShader>(filepath);
  default:
    PP_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
    return nullptr;
  }
}

Pepper::Ref<Pepper::Shader> Pepper::Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
{
  switch (Renderer::GetAPI())
  {
  case RendererAPI::API::OPEN_GL:
    return std::make_shared<OpenGLShader>(vertexSrc, fragmentSrc);
  default:
    PP_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
    return nullptr;
  }
}