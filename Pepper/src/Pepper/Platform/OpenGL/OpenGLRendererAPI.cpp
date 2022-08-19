//clang-format off
#include "PepperPCH.hpp"
//clang-format on

#include "OpenGLRendererAPI.hpp"

#include <glad/glad.h>

void Pepper::OpenGLRendererAPI::SetClearColor(const glm::vec4& color) const
{
  glClearColor(color.r, color.g, color.b, color.a);
}

void Pepper::OpenGLRendererAPI::Clear() const { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void Pepper::OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray) const
{
  glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
}