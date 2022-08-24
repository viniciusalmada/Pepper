// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "RenderCommand.hpp"

#include "Pepper/Platform/OpenGL/OpenGLRendererAPI.hpp"

namespace Pepper
{
  RendererAPI* RenderCommand::renderer_api = new OpenGLRendererAPI();
}
