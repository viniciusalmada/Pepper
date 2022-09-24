// clang-format off
#include <PepperPCH.hpp>
// clang-format on

#include "Player.hpp"

#include "Utilities.hpp"

#include <imgui.h>

Player::Player() {}

void Player::LoadAssets()
{
  PP_PROFILE_FUNCTION();
  m_rocket_tex = Pepper::Texture2D::Create("assets/textures/rocket.png");
  m_flame1_tex = Pepper::Texture2D::Create("assets/textures/flame1.png");
  m_flame2_tex = Pepper::Texture2D::Create("assets/textures/flame2.png");
  m_flame3_tex = Pepper::Texture2D::Create("assets/textures/flame3.png");
  m_flame_tex = m_flame1_tex;
}

void Player::OnUpdate(Pepper::TimeStep ts)
{
  PP_PROFILE_FUNCTION();
  if (!m_move)
    return;

  if (Pepper::Input::IsKeyPressed(PP_KEY_SPACE))
  {
    m_velocity.y += m_engine_power * ts;
    // m_rotation_deg += 3.5f * ts;
  }
  else
  {
    m_velocity.y -= m_gravity * ts;
    // m_rotation_deg -= 2.5f * ts;
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
}

void Player::OnImGuiLayer()
{
  PP_PROFILE_FUNCTION();
  ImGui::Checkbox("Move rocket", &m_move);
  ImGui::DragFloat("Engine Power", &m_engine_power, 1.0f, 1.0f, 50.0f);
  ImGui::DragFloat("Gravity", &m_gravity, 1.0f, 1.0f, 50.0f);
  ImGui::DragFloat("Rotation", &m_rotation_deg, 1.f, -180.f, 0.0f);
}

void Player::OnRendererCall()
{
  PP_PROFILE_FUNCTION();
  Pepper::Renderer2D::DrawRotatedQuad(m_position, m_rocket_size, m_rotation_deg, m_rocket_tex);
  Pepper::Renderer2D::DrawRotatedQuad({ m_position.x, m_position.y, 0.8 },
                                      m_flames_size,
                                      m_rotation_deg,
                                      m_flame_tex,
                                      1.0f,
                                      { 1.0f, 1.0f, 1.0f, m_flame_alpha * 2 });
}

const glm::vec3& Player::GetPosition() const
{
  PP_PROFILE_FUNCTION();
  return m_position;   
}
