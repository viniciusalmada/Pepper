// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Pepper/Renderer/ShaderLibrary.hpp"

namespace Pepper
{
  class ShaderLibrary::Impl
  {
  public:
    std::unordered_map<std::string, Ref<Shader>> shaders{};
  };

  ShaderLibrary::ShaderLibrary() : pimp(CreateScope<Impl>()) {}

  ShaderLibrary::~ShaderLibrary() = default;

  void ShaderLibrary::Add(const Ref<Shader>& shader)
  {
    auto name = shader->GetName();
    PP_CORE_ASSERT(!pimp->shaders.contains(name), "Shader already exists!");
    pimp->shaders[name] = shader;
  }

  Ref<Shader> ShaderLibrary::Load(const std::filesystem::path& filepath)
  {
    auto shader = Shader::Create(filepath);
    Add(shader);
    return shader;
  }

  Ref<Shader> ShaderLibrary::Get(const std::string& name) const
  {
    PP_CORE_ASSERT(pimp->shaders.contains(name), "Shader does not exist!");
    return pimp->shaders.at(name);
  }
} // namespace Pepper
