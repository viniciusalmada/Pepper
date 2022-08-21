//clang-format off
#include "PepperPCH.hpp"
//clang-format on

#include "OpenGLDebugger.hpp"
#include "OpenGLRendererAPI.hpp"

#include <glad/glad.h>

void Pepper::OpenGLRendererAPI::Init()
{
#ifdef PP_DEBUG
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(Pepper::OpenGLDebugger::Func, nullptr);
  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Pepper::OpenGLRendererAPI::SetClearColor(const glm::vec4& color) const
{
  glClearColor(color.r, color.g, color.b, color.a);
}

void Pepper::OpenGLRendererAPI::Clear() const { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void Pepper::OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray) const
{
  glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
}