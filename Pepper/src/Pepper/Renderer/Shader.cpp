// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Shader.hpp"

#include "Pepper/Platform/OpenGL/OpenGLShader.hpp"
#include "Renderer.hpp"

namespace Pepper
{
  Ref<Shader> Shader::Create(const std::filesystem::path& filepath)
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

  Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
  {
    switch (Renderer::GetAPI())
    {
    case RendererAPI::API::OPEN_GL:
      return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
    default:
      PP_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
      return nullptr;
    }
  }

  void ShaderLibrary::Add(const Ref<Shader>& shader)
  {
    auto name = shader->GetName();
    PP_CORE_ASSERT(!shaders.contains(name), "Shader already exists!");
    shaders[name] = shader;
  }

  Ref<Shader> ShaderLibrary::Load(const std::filesystem::path& filepath)
  {
    auto shader = Shader::Create(filepath);
    Add(shader);
    return shader;
  }

  Ref<Shader> ShaderLibrary::Get(const std::string& name) const
  {
    PP_CORE_ASSERT(shaders.contains(name), "Shader does not exist!");
    return shaders.at(name);
  }
}
