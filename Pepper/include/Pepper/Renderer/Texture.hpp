#pragma once

#include "Pepper/Core/Core.hpp"

#include <cstdint>
#include <filesystem>
#include <string>

namespace Pepper
{
  class Texture
  {
  public:
    virtual ~Texture() = default;

    virtual uint32_t GetWidth() const = 0;

    virtual uint32_t GetHeight() const = 0;

    virtual void Bind(uint32_t slot = 0) const = 0;
  };

  class Texture2D : public Texture
  {
  public:
    static Ref<Texture2D>
    Create(uint32_t width, uint32_t height, const std::vector<uint32_t>& data, uint32_t size);

    static Ref<Texture2D> Create(const std::filesystem::path& path);

    virtual bool operator==(const Texture2D& other) const = 0;
  };
}