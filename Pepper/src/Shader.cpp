// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Pepper/Renderer/Shader.hpp"

#include "Pepper/Platform/OpenGL/OpenGLShader.hpp"
#include "Pepper/Renderer/Renderer.hpp"

namespace Pepper
{
  Ref<Shader> Shader::Create(const std::filesystem::path& filepath)
  {
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::OPEN_GL:
      return std::make_shared<OpenGLShader>(filepath);
    case RendererAPI::API::NONE:
      PP_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
      return nullptr;
    }
    PP_CORE_ASSERT(false, "Unreachable code");
    return nullptr;
  }

  Ref<Shader> Shader::Create(const std::string& name,
                             const std::string& vertexSrc,
                             const std::string& fragmentSrc)
  {
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::OPEN_GL:
      return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
    case RendererAPI::API::NONE:
      PP_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
      return nullptr;
    }
    PP_CORE_ASSERT(false, "Unreachable code");
    return nullptr;
  }
}
