// clang-format off
#include <PepperPCH.hpp>
// clang-format on

#include "Player.hpp"

#include "Color.hpp"
#include "Constants.hpp"
#include "Random.hpp"
#include "Utilities.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

auto GetFlameColorBeginEnd()
{
  auto red_beg = Random::Float(226.0f, 252.0f) / 255.0f;
  auto green_beg = Random::Float(67.0f, 163.0f) / 255.0f;
  auto blue_beg = Random::Float(38.0f, 42.0f) / 255.0f;
  auto red_end = Random::Float(226.0f, 252.0f) / 255.0f;
  auto green_end = Random::Float(67.0f, 163.0f) / 255.0f;
  auto blue_end = Random::Float(38.0f, 42.0f) / 255.0f;

  return std::make_tuple(glm::vec4{ red_beg, green_beg, blue_beg, 1.0f },
                         glm::vec4{ red_end, green_end, blue_end, 1.0f });
}
auto GetSmokeColorBeginEnd()
{
  auto grey_beg = Random::Float(0.3f, 1.0f);
  auto grey_end = Random::Float(0.3f, 1.0f);

  return std::make_tuple(glm::vec4{ grey_beg, grey_beg, grey_beg, 1.0f },
                         glm::vec4{ grey_end, grey_end, grey_end, 1.0f });
}

Player::Player() = default;

void Player::Init()
{
  m_position = { 0.0f, 0.0f, Constants::Z_ROCKET };
  m_velocity = { 2.0f, 0.0f };
  m_rotation_deg = 0.0f;
  m_flame_alpha = 1.0f;
  m_move = false;

  LoadAssets();
}

void Player::LoadAssets()
{
  PP_PROFILE_FUNCTION()
  m_rocket_tex = Pepper::Texture2D::Create("assets/textures/rocket.png");
  m_flame_tex = Pepper::Texture2D::Create("assets/textures/flame3.png");
}

void Player::OnUpdate(Pepper::TimeStep ts)
{
  PP_PROFILE_FUNCTION()

  if (!m_move)
    return;

  m_time += ts;

  auto hipot = (m_rocket_size.y) / 2.0f;
  auto angle = glm::radians(std::abs(m_rotation_deg));
  auto x_disp = glm::sin(angle) * hipot;
  auto y_disp = glm::cos(angle) * hipot;
  auto particle_position = glm::vec2{ m_position.x - x_disp, m_position.y - y_disp };

  bool boost_pressed = false;
  if (Pepper::Input::IsKeyPressed(PP_KEY_SPACE))
  {
    boost_pressed = true;
    m_velocity.y += m_engine_power * ts;
  }
  else
  {
    m_velocity.y -= m_gravity * ts;
  }

  m_velocity.y = std::clamp(m_velocity.y, -20.0f, 20.0f);

  m_rotation_deg = Utils::Interpol(-20.0f, 20.0f, -180.0, 0.0, m_velocity.y);
  m_flame_alpha = Utils::Interpol(0.0f, 20.0f, 0.0, 1.0f, m_velocity.y);

  m_position.y += m_velocity.y * (float)ts;
  m_position.x += m_velocity.x * (float)ts;

  m_position.y = std::clamp(m_position.y, -5.625f - m_rocket_size.y, 5.625f + m_rocket_size.y);

  UpdateBoundingBox();

  if (boost_pressed && m_time > m_flame_next_emit_time)
  {
    auto [beg, end] = GetFlameColorBeginEnd();
    auto count = static_cast<uint32_t>(Utils::Interpol(-20.0f, 20.0f, 1.0f, 15.0f, m_velocity.y));
    m_particle_system.Emit(count, particle_position, beg, end);
    m_flame_next_emit_time += FLAME_EMIT_INTERVAL;
  }

  if (!boost_pressed && m_time > m_smoke_next_emit_time)
  {
    auto [beg, end] = GetSmokeColorBeginEnd();
    m_particle_system.Emit(4, particle_position, beg, end);
    m_smoke_next_emit_time += SMOKE_EMIT_INTERVAL;
  }
  m_particle_system.Update(ts);
}

void Player::OnImGuiLayer()
{
}

void Player::OnRendererCall()
{
  PP_PROFILE_FUNCTION()
  auto to_shader = glm::vec2{ 0.0, m_position.y / 11.25f };
  Pepper::Renderer2D::UploadVec2ToPixelShader(to_shader);

  Pepper::Renderer2D::DrawRotatedQuad(m_position, m_rocket_size, m_rotation_deg, m_rocket_tex);
  Pepper::Renderer2D::DrawRotatedQuad({ m_position.x, m_position.y, Constants::Z_ENGINE },
                                      m_flames_size,
                                      m_rotation_deg,
                                      m_flame_tex,
                                      1.0f,
                                      { 1.0f, 1.0f, 1.0f, m_flame_alpha * 2 });

  m_particle_system.OnRendererCall();
}

const glm::vec3& Player::GetPosition() const
{
  PP_PROFILE_FUNCTION()
  return m_position;
}

const std::array<glm::vec4, 4>& Player::GetBoundingBox() const
{
  return m_bounding_box;
}

void Player::Reset()
{
  m_position = { 0.0f, 0.0f, Constants::Z_ROCKET };
  m_velocity = { 2.0f, 0.0f };
  m_rotation_deg = 0.0f;
  m_flame_alpha = 1.0f;
  m_move = false;
  m_particle_system.Clear();
  m_time = 0.0f;
  m_flame_next_emit_time = FLAME_EMIT_INTERVAL;
  m_smoke_next_emit_time = SMOKE_EMIT_INTERVAL;
}

void Player::UpdateBoundingBox()
{
  static const glm::vec4 top_left =
    glm::vec4{ -m_rocket_size.x / 2.0f, m_rocket_size.y / 2.0f, 1.0f, 1.0f };
  static const glm::vec4 top_right = top_left + glm::vec4{ m_rocket_size.x, 0.0f, 1.0f, 1.0f };
  static const glm::vec4 bot_right = top_right + glm::vec4{ 0.0f, -m_rocket_size.y, 1.0f, 1.0f };
  static const glm::vec4 bot_left = bot_right + glm::vec4{ -m_rocket_size.x, 0.0f, 1.0f, 1.0f };

  auto calc_rot = [&](const glm::vec4& ref)
  {
    glm::mat4 rot_mat =
      glm::rotate(glm::mat4{ 1.0f }, glm::radians(m_rotation_deg), glm::vec3{ 0.0f, 0.0f, 1.0f });

    return rot_mat * glm::vec4{ ref.x, ref.y, 1.0f, 1.0f };
  };

  m_bounding_box[0] = calc_rot(top_left) + glm::vec4{ m_position.x, m_position.y, 1.0f, 1.0f };
  m_bounding_box[1] = calc_rot(top_right) + glm::vec4{ m_position.x, m_position.y, 1.0f, 1.0f };
  m_bounding_box[2] = calc_rot(bot_right) + glm::vec4{ m_position.x, m_position.y, 1.0f, 1.0f };
  m_bounding_box[3] = calc_rot(bot_left) + glm::vec4{ m_position.x, m_position.y, 1.0f, 1.0f };
}
void Player::StartMoving()
{
  m_move = true;
}
