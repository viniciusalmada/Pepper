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

    static Ref<Shader> Create(const std::filesystem::path& filepath);

    [[deprecated]] static Ref<Shader> Create(const std::string& vertexSrc, const std::string& fragmentSrc);
  };
}