// clang-format off
#include "PepperPCH.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_SIMD
#include <stb_image.h>
// clang-format on

#include "Pepper/Platform/OpenGL/OpenGLTexture.hpp"

#include <glad/glad.h>

namespace Pepper
{
  class OpenGLTexture2D::Impl
  {
  public:
    Impl(uint32_t width, uint32_t height, const std::vector<uint32_t>& data, uint32_t size);
    Impl(const std::filesystem::path& path);

    std::filesystem::path path;
    uint32_t width;
    uint32_t height;
    uint32_t renderer_ID;
  };

  OpenGLTexture2D::Impl::Impl(uint32_t width, uint32_t height, const std::vector<uint32_t>& data, uint32_t size) :
      width(width),
      height(height)
  {
    uint32_t internal_format = GL_RGBA8;
    uint32_t format = GL_RGBA;

    PP_CORE_ASSERT(size == width * height * 4, "Error at texture size!");

    PP_CORE_ASSERT(internal_format != 0, "Incorrect number of texture channels");
    PP_CORE_ASSERT(format != 0, "Incorrect number of texture channels");

    glCreateTextures(GL_TEXTURE_2D, 1, &renderer_ID);
    glTextureStorage2D(renderer_ID, 1, internal_format, width, height);

    glTextureParameteri(renderer_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(renderer_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureParameteri(renderer_ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(renderer_ID, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTextureSubImage2D(renderer_ID, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, data.data());
  }

  OpenGLTexture2D::Impl::Impl(const std::filesystem::path& path) : path(path)
  {
    int w{}, h{};
    int channels{};
    stbi_set_flip_vertically_on_load(1);
    stbi_uc* data = stbi_load(path.string().c_str(), &w, &h, &channels, 0);
    PP_CORE_ASSERT(data, "Failed to load image!");

    this->width = w;
    this->height = h;

    uint32_t internal_format = 0;
    uint32_t format = 0;
    if (channels == 4)
    {
      internal_format = GL_RGBA8;
      format = GL_RGBA;
    }
    else if (channels)
    {
      internal_format = GL_RGB8;
      format = GL_RGB;
    }

    PP_CORE_ASSERT(internal_format != 0, "Incorrect number of texture channels");
    PP_CORE_ASSERT(format != 0, "Incorrect number of texture channels");

    glCreateTextures(GL_TEXTURE_2D, 1, &renderer_ID);
    glTextureStorage2D(renderer_ID, 1, internal_format, width, height);

    glTextureParameteri(renderer_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(renderer_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureParameteri(renderer_ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(renderer_ID, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTextureSubImage2D(renderer_ID, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
  }

  OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, const std::vector<uint32_t>& data, uint32_t size) :
      pimp(CreateScope<Impl>(width, height, data, size))
  {
  }

  OpenGLTexture2D::OpenGLTexture2D(const std::filesystem::path& path) : pimp(CreateScope<Impl>(path)) {}

  OpenGLTexture2D::~OpenGLTexture2D()
  {
    glDeleteTextures(1, &pimp->renderer_ID);
  }

  uint32_t OpenGLTexture2D::GetWidth() const
  {
    return pimp->width;
  }

  uint32_t OpenGLTexture2D::GetHeight() const
  {
    return pimp->height;
  }

  void OpenGLTexture2D::Bind(uint32_t slot) const
  {
    glBindTextureUnit(slot, pimp->renderer_ID);
  }
}