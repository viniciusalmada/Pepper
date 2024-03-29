//clang-format off
#include "PepperPCH.hpp"
//clang-format on

#include "Pepper/Platform/OpenGL/OpenGLDebugger.hpp"
#include "Pepper/Platform/OpenGL/OpenGLRendererAPI.hpp"

#include <glad/glad.h>

namespace Pepper
{
  void OpenGLRendererAPI::Init()
  {
    PP_PROFILE_FUNCTION();
#ifdef PP_DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(OpenGLDebuggerFunc, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif
    glEnable(GL_MULTISAMPLE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
  }

  void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) const
  {
    PP_PROFILE_FUNCTION();
    glViewport(x, y, w, h);
  }

  void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) const
  {
    PP_PROFILE_FUNCTION();
    glClearColor(color.r, color.g, color.b, color.a);
  }

  void OpenGLRendererAPI::Clear() const
  {
    PP_PROFILE_FUNCTION();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray,
                                      uint32_t indexCount) const
  {
    PP_PROFILE_FUNCTION();
    glDrawElements(GL_TRIANGLES,
                   indexCount == 0 ? vertexArray->GetIndexBuffer()->GetCount() : indexCount,
                   GL_UNSIGNED_INT,
                   nullptr);
  }
}