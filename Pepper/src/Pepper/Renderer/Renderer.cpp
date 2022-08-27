// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Renderer.hpp"

#include "Pepper/Platform/OpenGL/OpenGLShader.hpp"
#include "RenderCommand.hpp"

namespace Pepper
{
  Renderer::SceneData* Renderer::scene_data = new Renderer::SceneData{};

  void Renderer::Init()
  {
    RenderCommand::Init();
  }

  void Renderer::BeginScene(const OrthoCamera& camera)
  {
    scene_data->view_proj_matrix = camera.GetViewProjectionMatrix();
  }

  void Renderer::EndScene() {}

  void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
  {
    shader->Bind();
    std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_view_projection",
                                                                       scene_data->view_proj_matrix);
    std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_transform", transform);

    vertexArray->Bind();
    RenderCommand::DrawIndexed(vertexArray);
  }
}