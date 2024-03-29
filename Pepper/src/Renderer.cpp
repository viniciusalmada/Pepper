// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Pepper/Renderer/Renderer.hpp"

#include "Pepper/Renderer/RenderCommand.hpp"
#include "Pepper/Renderer/Renderer2D.hpp"

namespace Pepper
{
  struct SceneData
  {
    glm::mat4 view_proj_matrix;
  };

  static Scope<SceneData> scene_data = CreateScope<SceneData>();

  void Renderer::Init()
  {
    RenderCommand::Init();
    Renderer2D::Init();
  }

  void Renderer::Shutdown()
  {
    Renderer2D::Shutdown();
  }

  void Renderer::OnViewportResize(uint32_t width, uint32_t height)
  {
    RenderCommand::SetViewport(0, 0, width, height);
  }

  void Renderer::BeginScene(const OrthoCamera& camera)
  {
    scene_data->view_proj_matrix = camera.GetViewProjectionMatrix();
  }

  void Renderer::EndScene() {}

  void Renderer::Submit(const Ref<Shader>& shader,
                        const Ref<VertexArray>& vertexArray,
                        const glm::mat4& transform)
  {
    shader->Bind();
    shader->SetMat4("u_view_projection", scene_data->view_proj_matrix);
    shader->SetMat4("u_transform", transform);

    vertexArray->Bind();
    RenderCommand::DrawIndexed(vertexArray);
  }
}