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

namespace Pepper
{
  struct QuadInfo
  {
    glm::vec3 position;
    glm::vec2 size;
    glm::vec4 color;
    Ref<Texture2D> texture = nullptr;
    float rotation_deg = 0;
  };

  struct VertexInfo
  {
    glm::vec3 position;
    glm::vec4 color;
    glm::vec2 tex_coords;
    float texture_id;
  };

  class RendererData
  {
  public:
    RendererData()
    {
      m_vertex_array = VertexArray::Create();
      m_vertex_array->Bind();

      m_vertex_buffer =
        VertexBuffer::Create(MAX_VERTICES * sizeof(VertexInfo), m_vertex_array->GetRendererID());
      m_vertex_buffer->SetLayout({
        {ShaderDataType::Float3,   "in_position"},
        {ShaderDataType::Float4,      "in_color"},
        {ShaderDataType::Float2, "in_tex_coords"},
        { ShaderDataType::Float, "in_texture_id"}
      });
      m_vertex_array->AddVertexBuffer(m_vertex_buffer);

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
      auto quad_ib = IndexBuffer::Create(square_indices, m_vertex_array->GetRendererID());
      m_vertex_array->SetIndexBuffer(quad_ib);

      m_shader = Shader::Create(R"(assets/shaders/ColorOrTexture.glsl)");
      m_shader->Bind();

      m_white_texture = Texture2D::Create(1, 1, { 0xFFFFFFFF }, sizeof(uint32_t));

      auto range = Views::iota(0, MAX_TEXTURE_SLOTS);
      std::vector<int> samplers{ range.begin(), range.end() };
      m_shader->SetIntArray("u_textures", samplers);

      m_textures_slots[m_current_texture++] = m_white_texture;

      m_quad_vertex_positions[0] = { -0.5, -0.5, 0.0f, 1.0f };
      m_quad_vertex_positions[1] = { +0.5, -0.5, 0.0f, 1.0f };
      m_quad_vertex_positions[2] = { +0.5, +0.5, 0.0f, 1.0f };
      m_quad_vertex_positions[3] = { -0.5, +0.5, 0.0f, 1.0f };
    }

    void PrepareBegin(const OrthoCamera& camera)
    {
      m_shader->Bind();
      m_shader->SetMat4("u_view_projection", camera.GetViewProjectionMatrix());

      m_quad_index_count = 0;
      m_current_vertex = 0;
    }

    void PrepareEnd()
    {
      auto data_size = static_cast<uint32_t>(m_current_vertex * sizeof(VertexInfo));
      m_vertex_buffer->UpdateData(m_vertices_data.data(), data_size);
      Draw();
    }

    void PushQuad(QuadInfo&& quad)
    {
      glm::mat4 transform_mat;
      if (quad.rotation_deg == 0.0f)
      {
        transform_mat = glm::translate(glm::mat4(1.0f), quad.position) *
                        glm::scale(glm::mat4(1.0f), { quad.size.x, quad.size.y, 1.0f });
      }
      else
      {
        transform_mat =
          glm::translate(glm::mat4(1.0f), quad.position) *
          glm::rotate(glm::mat4(1.0f), glm::radians(quad.rotation_deg), { 0.0f, 0.0f, 1.0f }) *
          glm::scale(glm::mat4(1.0f), { quad.size.x, quad.size.y, 1.0f });
      }
    }

  private:
    void Draw()
    {
      auto not_null = [](const Ref<Texture2D>& t) { return t != nullptr; };
      uint32_t i = 0;
      auto bind_tex = [&i](const Ref<Texture2D>& t) { t->Bind(i++); };
      Ranges::for_each(m_textures_slots | Views::values | Views::filter(not_null), bind_tex);

      RenderCommand::DrawIndexed(m_vertex_array, m_quad_index_count);
    }

    Pepper::Ref<Pepper::VertexArray> m_vertex_array = nullptr;
    Pepper::Ref<Pepper::VertexBuffer> m_vertex_buffer = nullptr;
    Pepper::Ref<Pepper::Shader> m_shader = nullptr;
    Pepper::Ref<Pepper::Texture2D> m_white_texture = nullptr;
    std::array<VertexInfo, MAX_VERTICES> m_vertices_data{};
    std::map<uint32_t, Pepper::Ref<Pepper::Texture2D>> m_textures_slots{};
    std::array<glm::vec4, 4> m_quad_vertex_positions{};

    uint32_t m_quad_index_count = 0;
    uint32_t m_current_vertex = 0;
    uint32_t m_current_texture = 0; // 0 = empty texture (white)
  };

  Pepper::Scope<RendererData> data;

  void Renderer2D::Init()
  {
    PP_PROFILE_FUNCTION()
    data = CreateScope<RendererData>();
  }

  void Renderer2D::Shutdown()
  {
    PP_PROFILE_FUNCTION()
    data.reset();
  }

  void Renderer2D::BeginScene(const OrthoCamera& camera)
  {
    PP_PROFILE_FUNCTION()
    data->PrepareBegin(camera);
  }

  void Renderer2D::EndScene()
  {
    PP_PROFILE_FUNCTION()
    data->PrepareEnd();
  }

  void
  Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
  {
    PP_PROFILE_FUNCTION()

    glm::mat4 transform_mat = glm::translate(glm::mat4(1.0f), position) *
                              glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    VertexInfo& bot_left = data->vertices_data[data->current_vertex++];
    bot_left.position = transform_mat * data->quad_vertex_positions[0];
    bot_left.color = color;
    bot_left.tex_coords = { 0.0f, 0.0f };
    bot_left.texture_id = 0;

    VertexInfo& bot_right = data->vertices_data[data->current_vertex++];
    bot_right.position = transform_mat * data->quad_vertex_positions[1];
    bot_right.color = color;
    bot_right.tex_coords = { 1.0f, 0.0f };
    bot_right.texture_id = 0;

    VertexInfo& top_right = data->vertices_data[data->current_vertex++];
    top_right.position = transform_mat * data->quad_vertex_positions[2];
    top_right.color = color;
    top_right.tex_coords = { 1.0f, 1.0f };
    top_right.texture_id = 0;

    VertexInfo& top_left = data->vertices_data[data->current_vertex++];
    top_left.position = transform_mat * data->quad_vertex_positions[3];
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

    VertexInfo& bot_left = data->vertices_data[data->current_vertex++];
    bot_left.position = transform_mat * data->quad_vertex_positions[0];
    bot_left.color = color;
    bot_left.tex_coords = { 0.0f, 0.0f };
    bot_left.texture_id = static_cast<float>(tex_id);

    VertexInfo& bot_right = data->vertices_data[data->current_vertex++];
    bot_right.position = transform_mat * data->quad_vertex_positions[1];
    bot_right.color = color;
    bot_right.tex_coords = { 1.0f, 0.0f };
    bot_right.texture_id = static_cast<float>(tex_id);

    VertexInfo& top_right = data->vertices_data[data->current_vertex++];
    top_right.position = transform_mat * data->quad_vertex_positions[2];
    top_right.color = color;
    top_right.tex_coords = { 1.0f, 1.0f };
    top_right.texture_id = static_cast<float>(tex_id);

    VertexInfo& top_left = data->vertices_data[data->current_vertex++];
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

    VertexInfo& bot_left = data->vertices_data[data->current_vertex++];
    bot_left.position = transform_mat * data->quad_vertex_positions[0];
    bot_left.color = color;
    bot_left.tex_coords = { 0.0f, 0.0f };
    bot_left.texture_id = 0;

    VertexInfo& bot_right = data->vertices_data[data->current_vertex++];
    bot_right.position = transform_mat * data->quad_vertex_positions[1];
    bot_right.color = color;
    bot_right.tex_coords = { 1.0f, 0.0f };
    bot_right.texture_id = 0;

    VertexInfo& top_right = data->vertices_data[data->current_vertex++];
    top_right.position = transform_mat * data->quad_vertex_positions[2];
    top_right.color = color;
    top_right.tex_coords = { 1.0f, 1.0f };
    top_right.texture_id = 0;

    VertexInfo& top_left = data->vertices_data[data->current_vertex++];
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

    VertexInfo& bot_left = data->vertices_data[data->current_vertex++];
    bot_left.position = transform_mat * data->quad_vertex_positions[0];
    bot_left.color = color;
    bot_left.tex_coords = { 0.0f, 0.0f };
    bot_left.texture_id = static_cast<float>(tex_id);

    VertexInfo& bot_right = data->vertices_data[data->current_vertex++];
    bot_right.position = transform_mat * data->quad_vertex_positions[1];
    bot_right.color = color;
    bot_right.tex_coords = { 1.0f, 0.0f };
    bot_right.texture_id = static_cast<float>(tex_id);

    VertexInfo& top_right = data->vertices_data[data->current_vertex++];
    top_right.position = transform_mat * data->quad_vertex_positions[2];
    top_right.color = color;
    top_right.tex_coords = { 1.0f, 1.0f };
    top_right.texture_id = static_cast<float>(tex_id);

    VertexInfo& top_left = data->vertices_data[data->current_vertex++];
    top_left.position = transform_mat * data->quad_vertex_positions[3];
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