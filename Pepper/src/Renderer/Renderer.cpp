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

void Pepper::Renderer::Submit(const std::shared_ptr<Shader>& shader,
                              const std::shared_ptr<VertexArray>& vertexArray,
                              const glm::mat4& transform)
{
  shader->Bind();
  shader->UploadUniformMat4("u_view_projection", scene_data->view_proj_matrix);
  shader->UploadUniformMat4("u_transform", transform);

  vertexArray->Bind();
  RenderCommand::DrawIndexed(vertexArray);
}
