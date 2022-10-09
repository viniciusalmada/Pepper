// clang-format off
#include <PepperPCH.hpp>
// clang-format on

#include "Player.hpp"

#include "Constants.hpp"
#include "Utilities.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

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
  m_flame1_tex = Pepper::Texture2D::Create("assets/textures/flame1.png");
  m_flame2_tex = Pepper::Texture2D::Create("assets/textures/flame2.png");
  m_flame3_tex = Pepper::Texture2D::Create("assets/textures/flame3.png");
  m_flame_tex = m_flame1_tex;
}

void Player::OnUpdate(Pepper::TimeStep ts)
{
  PP_PROFILE_FUNCTION()
  if (!m_move)
    return;

  if (Pepper::Input::IsKeyPressed(PP_KEY_SPACE))
  {
    m_velocity.y += m_engine_power * ts;
  }
  else
  {
    m_velocity.y -= m_gravity * ts;
  }

  m_rotation_deg = Utils::Interpol(-20.0f, 20.0f, -180.0, 0.0, m_velocity.y);
  m_flame_alpha = Utils::Interpol(-20.0f, 20.0f, 0.0, 1.0f, m_velocity.y);

  if (m_flame_alpha == std::clamp(m_flame_alpha, 0.0f, 0.3f))
  {
    m_flame_tex = m_flame1_tex;
  }
  else if (m_flame_alpha == std::clamp(m_flame_alpha, 0.3f, 0.6f))
  {
    m_flame_tex = m_flame2_tex;
  }
  else
  {
    m_flame_tex = m_flame3_tex;
  }

  m_velocity.y = std::clamp(m_velocity.y, -20.0f, 20.0f);

  m_position.y += m_velocity.y * (float)ts;
  m_position.x += m_velocity.x * (float)ts;

  m_position.y = std::clamp(m_position.y, -5.625f - m_rocket_size.y, 5.625f + m_rocket_size.y);

  UpdateBoundingBox();
}

void Player::OnImGuiLayer()
{
  PP_PROFILE_FUNCTION()
  ImGui::Checkbox("Move rocket", &m_move);
  ImGui::DragFloat("Rotation", &m_rotation_deg, 1.f, -180.f, 0.0f);
}

void Player::OnRendererCall()
{
  PP_PROFILE_FUNCTION()
  Pepper::Renderer2D::DrawRotatedQuad(m_position, m_rocket_size, m_rotation_deg, m_rocket_tex);
  Pepper::Renderer2D::DrawRotatedQuad({ m_position.x, m_position.y, Constants::Z_ENGINE },
                                      m_flames_size,
                                      m_rotation_deg,
                                      m_flame_tex,
                                      1.0f,
                                      { 1.0f, 1.0f, 1.0f, m_flame_alpha * 2 });
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
}

void Player::UpdateBoundingBox()
{
  static const glm::vec4 top_left = glm::vec4{ -m_rocket_size.x / 2.0f, m_rocket_size.y / 2.0f, 1.0f, 1.0f };
  static const glm::vec4 top_right = top_left + glm::vec4{ m_rocket_size.x, 0.0f, 1.0f, 1.0f };
  static const glm::vec4 bot_right = top_right + glm::vec4{ 0.0f, -m_rocket_size.y, 1.0f, 1.0f };
  static const glm::vec4 bot_left = bot_right + glm::vec4{ -m_rocket_size.x, 0.0f, 1.0f, 1.0f };

  auto calc_rot = [&](const glm::vec4& ref)
  {
    glm::mat4 rot_mat = glm::rotate(glm::mat4{ 1.0f }, glm::radians(m_rotation_deg), glm::vec3{ 0.0f, 0.0f, 1.0f });

    return rot_mat * glm::vec4{ ref.x, ref.y, 1.0f, 1.0f };
  };

  m_bounding_box[0] = calc_rot(top_left) + glm::vec4{ m_position.x, m_position.y, 1.0f, 1.0f };
  m_bounding_box[1] = calc_rot(top_right) + glm::vec4{ m_position.x, m_position.y, 1.0f, 1.0f };
  m_bounding_box[2] = calc_rot(bot_right) + glm::vec4{ m_position.x, m_position.y, 1.0f, 1.0f };
  m_bounding_box[3] = calc_rot(bot_left) + glm::vec4{ m_position.x, m_position.y, 1.0f, 1.0f };
}
