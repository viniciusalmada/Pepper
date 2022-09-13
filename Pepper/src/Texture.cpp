// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Pepper/Renderer/Texture.hpp"

#include "Pepper/Platform/OpenGL/OpenGLTexture.hpp"
#include "Pepper/Renderer/RendererAPI.hpp"

namespace Pepper
{
  Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height, const std::vector<uint32_t>& data, uint32_t size)
  {
    switch (RendererAPI::GetAPI())
    {
    case RendererAPI::API::OPEN_GL:
      return CreateRef<OpenGLTexture2D>(width, height, data, size);
    default:
      PP_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
      return nullptr;
    }
  }

  Ref<Texture2D> Texture2D::Create(const std::filesystem::path& path)
  {
    switch (RendererAPI::GetAPI())
    {
    case RendererAPI::API::OPEN_GL:
      return CreateRef<OpenGLTexture2D>(path);
    default:
      PP_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
      return nullptr;
    }
  }
}