// clang-format off
#include <PepperPCH.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
// clang-format on

#include "OpenGLTexture.hpp"

#include <glad/glad.h>

Pepper::OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : path(path)
{
  int w{}, h{};
  int channels{};
  stbi_set_flip_vertically_on_load(1);
  stbi_uc* data = stbi_load(path.c_str(), &w, &h, &channels, 0);
  PP_CORE_ASSERT(data, "Failed to load image!");

  this->width = w;
  this->height = h;

  glCreateTextures(GL_TEXTURE_2D, 1, &renderer_ID);
  glTextureStorage2D(renderer_ID, 1, GL_RGB8, width, height);

  glTextureParameteri(renderer_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTextureParameteri(renderer_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTextureSubImage2D(renderer_ID, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

  stbi_image_free(data);
}

Pepper::OpenGLTexture2D::~OpenGLTexture2D() { glDeleteTextures(1, &renderer_ID); }

void Pepper::OpenGLTexture2D::Bind(uint32_t slot) const { glBindTextureUnit(slot, renderer_ID); }
