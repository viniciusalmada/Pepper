#pragma once

#include "Pepper/Renderer/Texture.hpp"

namespace Pepper
{
  class OpenGLTexture2D : public Texture2D
  {
  public:
    OpenGLTexture2D(uint32_t width, uint32_t height, const std::vector<uint32_t>& data, uint32_t size);
    OpenGLTexture2D(const std::filesystem::path& path);

    ~OpenGLTexture2D();

    uint32_t GetWidth() const override;

    uint32_t GetHeight() const override;

    void Bind(uint32_t slot) const override;

  private:
    DECLARE_PIMPL
  };
}
