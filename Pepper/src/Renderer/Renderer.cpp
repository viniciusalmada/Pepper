// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Renderer.hpp"

#include "RenderCommand.hpp"

void Pepper::Renderer::BeginScene() {}

void Pepper::Renderer::EndScene() {}

void Pepper::Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
{
  vertexArray->Bind();
  RenderCommand::DrawIndexed(vertexArray);
}
