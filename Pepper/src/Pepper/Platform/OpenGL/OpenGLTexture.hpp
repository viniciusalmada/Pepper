#pragma once

#include "Pepper/Renderer/Texture.hpp"

namespace Pepper
{
  class OpenGLTexture2D : public Texture2D
  {
  public:
    OpenGLTexture2D(const std::filesystem::path& path);

    ~OpenGLTexture2D();

    uint32_t GetWidth() const override { return width; }

    uint32_t GetHeight() const override { return height; }

    void Bind(uint32_t slot) const override;

  private:
    std::filesystem::path path;
    uint32_t width;
    uint32_t height;
    uint32_t renderer_ID;
  };
}
