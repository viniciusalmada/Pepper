#pragma once

#include <filesystem>
#include <glm/glm.hpp>
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

    virtual void SetInt(const std::string&, const int&) = 0;
    virtual void SetMat4(const std::string&, const glm::mat4&) = 0;
    virtual void SetFloat4(const std::string&, const glm::vec4&) = 0;

    static Ref<Shader> Create(const std::filesystem::path& filepath);
    static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
  };
}