// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "RenderCommand.hpp"

#include "Platform/OpenGL/OpenGLRendererAPI.hpp"

Pepper::RendererAPI* Pepper::RenderCommand::renderer_api = new OpenGLRendererAPI();