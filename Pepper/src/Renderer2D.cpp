// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Pepper/Renderer/Renderer2D.hpp"

#include "Pepper/Renderer/RenderCommand.hpp"
#include "Pepper/Renderer/Shader.hpp"
#include "Pepper/Renderer/VertexArray.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Ranges = std::ranges;
namespace Views = std::views;

constexpr auto VERTEX_PER_QUAD = 6;
constexpr auto MAX_QUADS = 10'000;
constexpr auto MAX_VERTICES = MAX_QUADS * 4;
constexpr auto MAX_INDICES = MAX_QUADS * VERTEX_PER_QUAD;
constexpr auto MAX_TEXTURE_SLOTS = 32;

namespace
{
  struct QuadVertex
  {
    glm::vec3 position;
    glm::vec4 color;
    glm::vec2 tex_coords;
    float texture_id;
  };

  struct RendererData
  {
    Pepper::Ref<Pepper::VertexArray> vertex_array;
    Pepper::Ref<Pepper::VertexBuffer> vertex_buffer;
    Pepper::Ref<Pepper::Shader> shader;
    Pepper::Ref<Pepper::Texture2D> white_texture;

    uint32_t quad_index_count = 0;
    std::array<QuadVertex, MAX_VERTICES> vertices_data{};
    uint32_t current_vertex = 0;

    std::map<uint32_t, Pepper::Ref<Pepper::Texture2D>> textures_slots{};
    uint32_t current_texture = 0; // 0 = empty texture (white)

    std::array<glm::vec4, 4> quad_vertex_positions;
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
      {ShaderDataType::Float2, "in_tex_coords"},
      { ShaderDataType::Float, "in_texture_id"}
    });
    data->vertex_array->AddVertexBuffer(data->vertex_buffer);

    std::vector<uint32_t> square_indices(MAX_INDICES);
    uint32_t offset = 0;
    for (auto i = 0u; i < MAX_VERTICES; i += VERTEX_PER_QUAD)
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

    auto range = Views::iota(0, MAX_TEXTURE_SLOTS);
    std::vector<int> samplers{ range.begin(), range.end() };
    data->shader->SetIntArray("u_textures", samplers);

    data->textures_slots[data->current_texture++] = data->white_texture;

    data->quad_vertex_positions[0] = { -0.5, -0.5, 0.0f, 1.0f };
    data->quad_vertex_positions[1] = { +0.5, -0.5, 0.0f, 1.0f };
    data->quad_vertex_positions[2] = { +0.5, +0.5, 0.0f, 1.0f };
    data->quad_vertex_positions[3] = { -0.5, +0.5, 0.0f, 1.0f };
  }

  void Renderer2D::Shutdown()
  {
    PP_PROFILE_FUNCTION()
    data.reset();
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
    data->vertex_buffer->UpdateData(data->vertices_data.data(), data_size);

    Flush();
  }

  void Renderer2D::Flush()
  {
    auto not_null = [](const Ref<Texture2D>& t) { return t != nullptr; };
    uint32_t i = 0;
    auto bind_tex = [&i](const Ref<Texture2D>& t) { t->Bind(i++); };
    Ranges::for_each(data->textures_slots | Views::values | Views::filter(not_null), bind_tex);

    RenderCommand::DrawIndexed(data->vertex_array, data->quad_index_count);
  }

  void
  Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
  {
    PP_PROFILE_FUNCTION()

    glm::mat4 transform_mat = glm::translate(glm::mat4(1.0f), position) *
                              glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    QuadVertex& bot_left = data->vertices_data[data->current_vertex++];
    bot_left.position = transform_mat * data->quad_vertex_positions[0];
    bot_left.color = color;
    bot_left.tex_coords = { 0.0f, 0.0f };
    bot_left.texture_id = 0;

    QuadVertex& bot_right = data->vertices_data[data->current_vertex++];
    bot_right.position = transform_mat * data->quad_vertex_positions[1];
    bot_right.color = color;
    bot_right.tex_coords = { 1.0f, 0.0f };
    bot_right.texture_id = 0;

    QuadVertex& top_right = data->vertices_data[data->current_vertex++];
    top_right.position = transform_mat * data->quad_vertex_positions[2];
    top_right.color = color;
    top_right.tex_coords = { 1.0f, 1.0f };
    top_right.texture_id = 0;

    QuadVertex& top_left = data->vertices_data[data->current_vertex++];
    top_left.position = transform_mat * data->quad_vertex_positions[3];
    ;
    top_left.color = color;
    top_left.tex_coords = { 0.0f, 1.0f };
    top_left.texture_id = 0;

    data->quad_index_count += VERTEX_PER_QUAD;
  }

  void
  Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
  {
    DrawQuad({ position.x, position.y, 0.0f }, size, color);
  }

  void Renderer2D::DrawQuad(const glm::vec3& position,
                            const glm::vec2& size,
                            const Ref<Texture2D>& quadTexture,
                            const glm::vec4&)
  {
    PP_PROFILE_FUNCTION()
    constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

    auto res = Ranges::find_if(
      data->textures_slots,
      [in_tex = quadTexture](const std::pair<const uint32_t, Ref<Texture2D>>& tex_pair)
      {
        if (tex_pair.second == nullptr)
          return false;
        return *tex_pair.second == *in_tex.get();
      });

    uint32_t tex_id = 0;
    if (res != data->textures_slots.end())
      tex_id = res->first;

    if (tex_id == 0)
    {
      if (data->current_texture < MAX_TEXTURE_SLOTS)
      {
        tex_id = data->current_texture;
        data->textures_slots[tex_id] = quadTexture;
        data->current_texture++;
      }
    }

    glm::mat4 transform_mat = glm::translate(glm::mat4(1.0f), position) *
                              glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    QuadVertex& bot_left = data->vertices_data[data->current_vertex++];
    bot_left.position = transform_mat * data->quad_vertex_positions[0];
    bot_left.color = color;
    bot_left.tex_coords = { 0.0f, 0.0f };
    bot_left.texture_id = static_cast<float>(tex_id);

    QuadVertex& bot_right = data->vertices_data[data->current_vertex++];
    bot_right.position = transform_mat * data->quad_vertex_positions[1];
    bot_right.color = color;
    bot_right.tex_coords = { 1.0f, 0.0f };
    bot_right.texture_id = static_cast<float>(tex_id);

    QuadVertex& top_right = data->vertices_data[data->current_vertex++];
    top_right.position = transform_mat * data->quad_vertex_positions[2];
    top_right.color = color;
    top_right.tex_coords = { 1.0f, 1.0f };
    top_right.texture_id = static_cast<float>(tex_id);

    QuadVertex& top_left = data->vertices_data[data->current_vertex++];
    top_left.position = transform_mat * data->quad_vertex_positions[3];
    top_left.color = color;
    top_left.tex_coords = { 0.0f, 1.0f };
    top_left.texture_id = static_cast<float>(tex_id);

    data->quad_index_count += VERTEX_PER_QUAD;
  }

  void Renderer2D::DrawQuad(const glm::vec2& position,
                            const glm::vec2& size,
                            const Ref<Texture2D>& tex,
                            const glm::vec4& tintColor)
  {
    DrawQuad({ position.x, position.y, 0.0f }, size, tex, tintColor);
  }

  void Renderer2D::DrawRotatedQuad(const glm::vec3& position,
                                   const glm::vec2& size,
                                   float rotationDeg,
                                   const glm::vec4& color)
  {
    PP_PROFILE_FUNCTION()
    glm::mat4 transform_mat =
      glm::translate(glm::mat4(1.0f), position) *
      glm::rotate(glm::mat4(1.0f), glm::radians(rotationDeg), { 0.0f, 0.0f, 1.0f }) *
      glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    QuadVertex& bot_left = data->vertices_data[data->current_vertex++];
    bot_left.position = transform_mat * data->quad_vertex_positions[0];
    bot_left.color = color;
    bot_left.tex_coords = { 0.0f, 0.0f };
    bot_left.texture_id = 0;

    QuadVertex& bot_right = data->vertices_data[data->current_vertex++];
    bot_right.position = transform_mat * data->quad_vertex_positions[1];
    bot_right.color = color;
    bot_right.tex_coords = { 1.0f, 0.0f };
    bot_right.texture_id = 0;

    QuadVertex& top_right = data->vertices_data[data->current_vertex++];
    top_right.position = transform_mat * data->quad_vertex_positions[2];
    top_right.color = color;
    top_right.tex_coords = { 1.0f, 1.0f };
    top_right.texture_id = 0;

    QuadVertex& top_left = data->vertices_data[data->current_vertex++];
    top_left.position = transform_mat * data->quad_vertex_positions[3];
    top_left.color = color;
    top_left.tex_coords = { 0.0f, 1.0f };
    top_left.texture_id = 0;

    data->quad_index_count += VERTEX_PER_QUAD;
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
                                   const Ref<Texture2D>& quadTexture,
                                   const glm::vec4&)
  {
    PP_PROFILE_FUNCTION()
    constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

    auto res = Ranges::find_if(
      data->textures_slots,
      [in_tex = quadTexture](const std::pair<const uint32_t, Ref<Texture2D>>& tex_pair)
      {
        if (tex_pair.second == nullptr)
          return false;
        return *tex_pair.second == *in_tex.get();
      });

    uint32_t tex_id = 0;
    if (res != data->textures_slots.end())
      tex_id = res->first;

    if (tex_id == 0)
    {
      if (data->current_texture < MAX_TEXTURE_SLOTS)
      {
        tex_id = data->current_texture;
        data->textures_slots[tex_id] = quadTexture;
        data->current_texture++;
      }
    }

    glm::mat4 transform_mat =
      glm::translate(glm::mat4(1.0f), position) *
      glm::rotate(glm::mat4(1.0f), glm::radians(rotationDeg), { 0.0f, 0.0f, 1.0f }) *
      glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    QuadVertex& bot_left = data->vertices_data[data->current_vertex++];
    bot_left.position = transform_mat * data->quad_vertex_positions[0];
    bot_left.color = color;
    bot_left.tex_coords = { 0.0f, 0.0f };
    bot_left.texture_id = static_cast<float>(tex_id);

    QuadVertex& bot_right = data->vertices_data[data->current_vertex++];
    bot_right.position = transform_mat * data->quad_vertex_positions[1];
    bot_right.color = color;
    bot_right.tex_coords = { 1.0f, 0.0f };
    bot_right.texture_id = static_cast<float>(tex_id);

    QuadVertex& top_right = data->vertices_data[data->current_vertex++];
    top_right.position = transform_mat * data->quad_vertex_positions[2];
    top_right.color = color;
    top_right.tex_coords = { 1.0f, 1.0f };
    top_right.texture_id = static_cast<float>(tex_id);

    QuadVertex& top_left = data->vertices_data[data->current_vertex++];
    top_left.position = transform_mat * data->quad_vertex_positions[3];
    ;
    top_left.color = color;
    top_left.tex_coords = { 0.0f, 1.0f };
    top_left.texture_id = static_cast<float>(tex_id);

    data->quad_index_count += VERTEX_PER_QUAD;
  }

  void Renderer2D::DrawRotatedQuad(const glm::vec2& position,
                                   const glm::vec2& size,
                                   float rotationDeg,
                                   const Ref<Texture2D>& tex,
                                   const glm::vec4& tintColor)
  {
    DrawRotatedQuad(glm::vec3{ position.x, position.y, 0.0f }, size, rotationDeg, tex, tintColor);
  }

}