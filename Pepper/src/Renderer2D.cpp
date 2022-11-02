// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Pepper/Renderer/Renderer2D.hpp"

#include "Pepper/Renderer/RenderCommand.hpp"
#include "Pepper/Renderer/Shader.hpp"
#include "Pepper/Renderer/VertexArray.hpp"

#include <glm/gtc/matrix_transform.hpp>

constexpr auto MAX_QUADS = 10'000;
constexpr auto MAX_VERTICES = MAX_QUADS * 4;
constexpr auto MAX_INDICES = MAX_QUADS * 6;

namespace
{
  struct QuadVertex
  {
    glm::vec3 position;
    glm::vec4 color;
    glm::vec2 tex_coords;
  };

  struct RendererData
  {
    Pepper::Ref<Pepper::VertexArray> vertex_array;
    Pepper::Ref<Pepper::VertexBuffer> vertex_buffer;
    Pepper::Ref<Pepper::Shader> shader;
    Pepper::Ref<Pepper::Texture2D> white_texture;

    uint32_t quad_index_count = 0;
    std::array<QuadVertex, MAX_VERTICES> vertex_buffer_base{};
    uint32_t current_vertex = 0;
  };

  Pepper::Scope<RendererData> data;
}

namespace Pepper
{
  void Renderer2D::Init()
  {
    PP_PROFILE_FUNCTION()
    data = CreateScope<RendererData>();
    data->vertex_array = VertexArray::Create();
    data->vertex_array->Bind();

    data->vertex_buffer =
      VertexBuffer::Create(MAX_VERTICES * sizeof(QuadVertex), data->vertex_array->GetRendererID());
    data->vertex_buffer->SetLayout({
      {ShaderDataType::Float3,   "in_position"},
      {ShaderDataType::Float4,      "in_color"},
      {ShaderDataType::Float2, "in_tex_coords"}
    });
    data->vertex_array->AddVertexBuffer(data->vertex_buffer);

    std::vector<uint32_t> square_indices(MAX_INDICES);
    uint32_t offset = 0;
    for (auto i = 0u; i < MAX_VERTICES; i += 6)
    {
      square_indices[i + 0] = offset + 0;
      square_indices[i + 1] = offset + 1;
      square_indices[i + 2] = offset + 2;

      square_indices[i + 3] = offset + 2;
      square_indices[i + 4] = offset + 3;
      square_indices[i + 5] = offset + 0;

      offset += 4;
    }
    auto quad_ib = IndexBuffer::Create(square_indices, data->vertex_array->GetRendererID());
    data->vertex_array->SetIndexBuffer(quad_ib);

    data->shader = Shader::Create(R"(assets/shaders/ColorOrTexture.glsl)");
    data->shader->Bind();

    data->white_texture = Texture2D::Create(1, 1, { 0xFFFFFFFF }, sizeof(uint32_t));
  }

  void Renderer2D::Shutdown()
  {
    PP_PROFILE_FUNCTION()
    // Free renderer data
  }

  void Renderer2D::BeginScene(const OrthoCamera& camera)
  {
    PP_PROFILE_FUNCTION()
    data->shader->Bind();
    data->shader->SetMat4("u_view_projection", camera.GetViewProjectionMatrix());

    data->quad_index_count = 0;
    data->current_vertex = 0;
  }

  void Renderer2D::EndScene()
  {
    PP_PROFILE_FUNCTION()

    auto data_size = static_cast<uint32_t>(data->current_vertex * sizeof(QuadVertex));
    data->vertex_buffer->UpdateData(data->vertex_buffer_base.data(), data_size);

    Flush();
  }

  void Renderer2D::Flush()
  {
    data->vertex_array->Bind();
    RenderCommand::DrawIndexed(data->vertex_array, data->quad_index_count);
  }

  void
  Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
  {
    PP_PROFILE_FUNCTION()

    QuadVertex& bot_left = data->vertex_buffer_base[data->current_vertex++];
    bot_left.position = { position.x - 0.5f * size.x, position.y - 0.5f * size.y, position.z };
    bot_left.color = color;
    bot_left.tex_coords = { 0.0f, 0.0f };

    QuadVertex& bot_right = data->vertex_buffer_base[data->current_vertex++];
    bot_right.position = { position.x + 0.5f * size.x, position.y - 0.5f * size.y, position.z };
    bot_right.color = color;
    bot_right.tex_coords = { 1.0f, 0.0f };

    QuadVertex& top_right = data->vertex_buffer_base[data->current_vertex++];
    top_right.position = { position.x + 0.5f * size.x, position.y + 0.5f * size.y, position.z };
    top_right.color = color;
    top_right.tex_coords = { 1.0f, 1.0f };

    QuadVertex& top_left = data->vertex_buffer_base[data->current_vertex++];
    top_left.position = { position.x - 0.5f * size.x, position.y + 0.5f * size.y, position.z };
    top_left.color = color;
    top_left.tex_coords = { 0.0f, 1.0f };

    data->quad_index_count += 6;

    //    data->shader->SetFloat("u_tiling_factor", 1.0);
    //    data->white_texture->Bind();
    //
    //    glm::mat4 transform = glm::translate(glm::mat4{ 1.0f }, position) *
    //                          glm::scale(glm::mat4{ 1.0f }, glm::vec3{ size.x, size.y, 1.0f });
    //    data->shader->SetMat4("u_transform", transform);
    //
    //    data->vertex_array->Bind();
    //    RenderCommand::DrawIndexed(data->vertex_array);
  }

  void
  Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
  {
    DrawQuad({ position.x, position.y, 0.0f }, size, color);
  }

  void Renderer2D::DrawQuad(const glm::vec3& position,
                            const glm::vec2& size,
                            const Ref<Texture2D>& tex,
                            float tilingFac,
                            const glm::vec4& tintColor)
  {
    PP_PROFILE_FUNCTION()
    tex->Bind();
    data->shader->SetFloat4("u_color", tintColor);
    data->shader->SetFloat("u_tiling_factor", tilingFac);

    glm::mat4 transform = glm::translate(glm::mat4{ 1.0f }, position) *
                          glm::scale(glm::mat4{ 1.0f }, glm::vec3{ size.x, size.y, 1.0f });
    data->shader->SetMat4("u_transform", transform);

    data->vertex_array->Bind();
    RenderCommand::DrawIndexed(data->vertex_array);
  }

  void Renderer2D::DrawQuad(const glm::vec2& position,
                            const glm::vec2& size,
                            const Ref<Texture2D>& tex,
                            float tilingFac,
                            const glm::vec4& tintColor)
  {
    DrawQuad({ position.x, position.y, 0.0f }, size, tex, tilingFac, tintColor);
  }

  void Renderer2D::DrawRotatedQuad(const glm::vec3& position,
                                   const glm::vec2& size,
                                   float rotationDeg,
                                   const glm::vec4& color)
  {
    PP_PROFILE_FUNCTION()
    data->shader->SetFloat4("u_color", color);
    data->shader->SetFloat("u_tiling_factor", 1.0);
    data->white_texture->Bind();

    glm::mat4 transform =
      glm::translate(glm::mat4{ 1.0f }, position) *
      glm::rotate(glm::mat4{ 1.0f }, glm::radians(rotationDeg), glm::vec3{ 0.0, 0.0, 1.0f }) *
      glm::scale(glm::mat4{ 1.0f }, glm::vec3{ size.x, size.y, 1.0f });
    data->shader->SetMat4("u_transform", transform);

    data->vertex_array->Bind();
    RenderCommand::DrawIndexed(data->vertex_array);
  }

  void Renderer2D::DrawRotatedQuad(const glm::vec2& position,
                                   const glm::vec2& size,
                                   float rotationDeg,
                                   const glm::vec4& color)
  {
    DrawRotatedQuad(glm::vec3{ position.x, position.y, 0.0f }, size, rotationDeg, color);
  }

  void Renderer2D::DrawRotatedQuad(const glm::vec3& position,
                                   const glm::vec2& size,
                                   float rotationDeg,
                                   const Ref<Texture2D>& tex,
                                   float tilingFac,
                                   const glm::vec4& tintColor)
  {
    PP_PROFILE_FUNCTION()
    tex->Bind();
    data->shader->SetFloat4("u_color", tintColor);
    data->shader->SetFloat("u_tiling_factor", tilingFac);

    glm::mat4 transform =
      glm::translate(glm::mat4{ 1.0f }, position) *
      glm::rotate(glm::mat4{ 1.0f }, glm::radians(rotationDeg), glm::vec3{ 0.0, 0.0, 1.0f }) *
      glm::scale(glm::mat4{ 1.0f }, glm::vec3{ size.x, size.y, 1.0f });
    data->shader->SetMat4("u_transform", transform);

    data->vertex_array->Bind();
    RenderCommand::DrawIndexed(data->vertex_array);
  }

  void Renderer2D::DrawRotatedQuad(const glm::vec2& position,
                                   const glm::vec2& size,
                                   float rotationDeg,
                                   const Ref<Texture2D>& tex,
                                   float tilingFac,
                                   const glm::vec4& tintColor)
  {
    DrawRotatedQuad(glm::vec3{ position.x, position.y, 0.0f },
                    size,
                    rotationDeg,
                    tex,
                    tilingFac,
                    tintColor);
  }

}