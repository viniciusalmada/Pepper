#pragma once

#include "Pepper/Core/Core.hpp"
#include "Shader.hpp"

#include <filesystem>

namespace Pepper
{
  class ShaderLibrary
  {
  public:
    ShaderLibrary();
    ~ShaderLibrary();

    void Add(const Ref<Shader>&);
    Ref<Shader> Load(const std::filesystem::path&);

    Ref<Shader> Get(const std::string&) const;

  private:
    DECLARE_PIMPL
  };
}