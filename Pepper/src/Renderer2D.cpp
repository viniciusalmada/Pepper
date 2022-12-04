// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Pepper/Renderer/Renderer2D.hpp"

#include "Pepper/Renderer/RenderCommand.hpp"
#include "Pepper/Renderer/Shader.hpp"
#include "Pepper/Renderer/VertexArray.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <utility>

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
    explicit QuadInfo(const glm::vec3& pos,
                      const glm::vec2 size,
                      const glm::vec4 col,
                      Ref<Texture2D> tex = nullptr,
                      float rotDeg = 0.0f) :
        position(pos),
        size(size),
        color(col),
        texture(std::move(tex)),
        rotation_deg(rotDeg)
    {
    }

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

      uint32_t texture_slot = 0;
      if (quad.texture != nullptr)
      {
        auto [texture_pushed, slot] = IsTexturePushed(quad.texture);
        if (texture_pushed)
          texture_slot = slot;
        else
          texture_slot = PushTextureAtFreeSlot(quad.texture);
      }

      VertexInfo& bot_left = m_vertices_data[m_current_vertex++];
      bot_left.position = transform_mat * m_quad_vertex_positions[0];
      bot_left.color = quad.color;
      bot_left.tex_coords = { 0.0f, 0.0f };
      bot_left.texture_id = static_cast<float>(texture_slot);

      VertexInfo& bot_right = m_vertices_data[m_current_vertex++];
      bot_right.position = transform_mat * m_quad_vertex_positions[1];
      bot_right.color = quad.color;
      bot_right.tex_coords = { 1.0f, 0.0f };
      bot_right.texture_id = static_cast<float>(texture_slot);

      VertexInfo& top_right = m_vertices_data[m_current_vertex++];
      top_right.position = transform_mat * m_quad_vertex_positions[2];
      top_right.color = quad.color;
      top_right.tex_coords = { 1.0f, 1.0f };
      top_right.texture_id = static_cast<float>(texture_slot);

      VertexInfo& top_left = m_vertices_data[m_current_vertex++];
      top_left.position = transform_mat * m_quad_vertex_positions[3];
      top_left.color = quad.color;
      top_left.tex_coords = { 0.0f, 1.0f };
      top_left.texture_id = static_cast<float>(texture_slot);

      m_quad_index_count += VERTEX_PER_QUAD;
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

    std::tuple<bool, uint32_t> IsTexturePushed(const Ref<Texture2D>& value)
    {
      auto cmp_textures =
        [in_tex = value](const std::pair<uint32_t, Ref<Pepper::Texture2D>>& texture_pair)
      {
        if (texture_pair.second == nullptr)
          return false;
        return *texture_pair.second == *in_tex.get();
      };

      auto res = Ranges::find_if(m_textures_slots, cmp_textures);

      if (res != m_textures_slots.end())
        return { true, res->first };
      else
        return { false, 0 };
    }

    [[nodiscard]] uint32_t PushTextureAtFreeSlot(const Ref<Texture2D>& value)
    {
      if (m_current_texture < MAX_TEXTURE_SLOTS)
      {
        m_textures_slots[m_current_texture] = value;
        return m_current_texture++;
      }
      return 0;
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
    data->PushQuad(QuadInfo{ position, size, color });
  }

  void
  Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
  {
    DrawQuad({ position.x, position.y, 0.0f }, size, color);
  }

  void Renderer2D::DrawQuad(const glm::vec3& position,
                            const glm::vec2& size,
                            const Ref<Texture2D>& quadTexture,
                            const glm::vec4& color)
  {
    PP_PROFILE_FUNCTION()
    data->PushQuad(QuadInfo{ position, size, color, quadTexture });
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
    data->PushQuad(QuadInfo{ position, size, color, nullptr, rotationDeg });
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
                                   const glm::vec4& color)
  {
    PP_PROFILE_FUNCTION()
    data->PushQuad(QuadInfo{ position, size, color, quadTexture, rotationDeg });
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