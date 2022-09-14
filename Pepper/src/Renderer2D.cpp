// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Pepper/Renderer/Renderer2D.hpp"

#include "Pepper/Renderer/RenderCommand.hpp"
#include "Pepper/Renderer/Shader.hpp"
#include "Pepper/Renderer/VertexArray.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Pepper
{
  struct RendererData
  {
    Ref<VertexArray> quad_vertex_array;
    Ref<Shader> shader;
    Ref<Texture2D> white_texture;
  };

  static Scope<RendererData> data;

  void Renderer2D::Init()
  {
    data = CreateScope<RendererData>();
    data->quad_vertex_array = VertexArray::Create();
    data->quad_vertex_array->Bind();

    // clang-format off
    std::vector<float> square_vertices = { 
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bot-left
      +0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bot-right
      +0.5f, +0.5f, 0.0f, 1.0f, 1.0f, // top-right
      -0.5f, +0.5f, 0.0f, 0.0f, 1.0f, // top-left
    };
    // clang-format on
    Ref<VertexBuffer> square_vb = VertexBuffer::Create(square_vertices, data->quad_vertex_array->GetRendererID());
    square_vb->SetLayout({ { ShaderDataType::Float3, "in_position" }, { ShaderDataType::Float2, "in_tex_coords" } });
    data->quad_vertex_array->AddVertexBuffer(square_vb);

    std::vector<uint32_t> square_indices = { 0, 1, 2, 2, 3, 0 };
    Ref<IndexBuffer> square_ib = IndexBuffer::Create(square_indices, data->quad_vertex_array->GetRendererID());
    data->quad_vertex_array->SetIndexBuffer(square_ib);

    data->shader = Shader::Create(R"(assets/shaders/ColorOrTexture.glsl)");
    data->shader->Bind();
    data->shader->SetInt("u_texture", 0);

    data->white_texture = Texture2D::Create(1, 1, { 0xFFFFFFFF }, sizeof(uint32_t));
  }

  void Renderer2D::Shutdown()
  {
    data.reset();
  }

  void Renderer2D::BeginScene(const OrthoCamera& camera)
  {
    data->shader->Bind();
    data->shader->SetMat4("u_view_projection", camera.GetViewProjectionMatrix());
  }

  void Renderer2D::EndScene() {}

  void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
  {
    data->shader->SetFloat4("u_color", color);
    data->white_texture->Bind();

    glm::mat4 transform = glm::translate(glm::mat4{ 1.0f }, position) *
                          glm::scale(glm::mat4{ 1.0f }, glm::vec3{ size.x * 2.0f, size.y * 2.0f, 1.0f });
    data->shader->SetMat4("u_transform", transform);

    data->quad_vertex_array->Bind();
    RenderCommand::DrawIndexed(data->quad_vertex_array);
  }

  void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
  {
    DrawQuad({ position.x, position.y, 0.0f }, size, color);
  }

  void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& tex)
  {
    tex->Bind();
    data->shader->SetFloat4("u_color", { 1.0f, 1.0f, 1.0f, 1.0f });

    glm::mat4 transform = glm::translate(glm::mat4{ 1.0f }, position) *
                          glm::scale(glm::mat4{ 1.0f }, glm::vec3{ size.x * 2.0f, size.y * 2.0f, 1.0f });
    data->shader->SetMat4("u_transform", transform);

    data->quad_vertex_array->Bind();
    RenderCommand::DrawIndexed(data->quad_vertex_array);
  }

  void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& tex)
  {
    DrawQuad({ position.x, position.y, 0.0f }, size, tex);
  }

}