// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Renderer.hpp"

#include "RenderCommand.hpp"

Pepper::Renderer::SceneData* Pepper::Renderer::scene_data = new Pepper::Renderer::SceneData{};

void Pepper::Renderer::BeginScene(OrthoCamera& camera)
{
  scene_data->view_proj_matrix = camera.GetViewProjectionMatrix();
}

void Pepper::Renderer::EndScene() {}

void Pepper::Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray)
{
  shader->Bind();
  shader->UploadUniformMat4("u_view_projection", scene_data->view_proj_matrix);

  vertexArray->Bind();
  RenderCommand::DrawIndexed(vertexArray);
}
