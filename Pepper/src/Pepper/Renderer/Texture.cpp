// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Texture.hpp"

#include "Pepper/Platform/OpenGL/OpenGLTexture.hpp"
#include "RendererAPI.hpp"

Pepper::Ref<Pepper::Texture2D> Pepper::Texture2D::Create(const std::filesystem::path& path)
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