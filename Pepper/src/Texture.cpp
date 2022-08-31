// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Pepper/Renderer/Texture.hpp"

#include "Pepper/Platform/OpenGL/OpenGLTexture.hpp"
#include "Pepper/Renderer/RendererAPI.hpp"

namespace Pepper
{
  Ref<Texture2D> Texture2D::Create(const std::filesystem::path& path)
  {
    switch (RendererAPI::GetAPI())
    {
    case RendererAPI::API::OPEN_GL:
      return Ref<Texture2D>(new OpenGLTexture2D(path));
    default:
      PP_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
      return nullptr;
    }
  }
}