// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Pepper/Renderer/RenderCommand.hpp"

#include "Pepper/Platform/OpenGL/OpenGLRendererAPI.hpp"

namespace Pepper
{
  Scope<RendererAPI> RenderCommand::renderer_api = CreateScope<OpenGLRendererAPI>();
}
