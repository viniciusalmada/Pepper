#pragma once

#include <filesystem>
#include <string>

namespace Pepper
{
  class Shader
  {
  public:
    virtual ~Shader() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual const std::string& GetName() const = 0;

    static Ref<Shader> Create(const std::filesystem::path& filepath);
    [[deprecated]] static Ref<Shader> Create(const std::string& vertexSrc, const std::string& fragmentSrc);
  };

  class ShaderLibrary
  {
  public:
    void Add(const Ref<Shader>& shader);
    Ref<Shader> Load(const std::filesystem::path& filepath);

    Ref<Shader> Get(const std::string& name) const;

  private:
    std::unordered_map<std::string, Ref<Shader>> shaders;
  };
}