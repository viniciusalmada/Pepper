// clang-format off
#include <PepperPCH.hpp>
// clang-format on

#include "Player.hpp"

#include "Utilities.hpp"

#include <imgui.h>

Player::Player() {}

void Player::LoadAssets()
{
  m_rocket_tex = Pepper::Texture2D::Create("assets/textures/rocket.png");
  m_flame1_tex = Pepper::Texture2D::Create("assets/textures/flame1.png");
  m_flame2_tex = Pepper::Texture2D::Create("assets/textures/flame2.png");
  m_flame3_tex = Pepper::Texture2D::Create("assets/textures/flame3.png");
  m_flame_tex = m_flame1_tex;
}

void Player::OnUpdate(Pepper::TimeStep ts)
{
  if (!m_move)
    return;

  if (Pepper::Input::IsKeyPressed(PP_KEY_SPACE))
  {
    m_velocity.y += m_engine_power;
    m_rotation_deg += 3.5f;
  }
  else
  {
    m_velocity.y -= m_gravity;
    m_rotation_deg -= 2.5f;
  }
  m_rotation_deg = std::clamp(m_rotation_deg, -180.0f, 0.0f);
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
  m_position += m_velocity * (float)ts;

  m_position.y = std::clamp(m_position.y, -5.625f - m_rocket_size.y, 5.625f + m_rocket_size.y);
}

void Player::OnImGuiLayer()
{
  ImGui::Checkbox("Move rocket", &m_move);
  ImGui::DragFloat("Engine Power", &m_engine_power, 0.05f, 0.05f, 20.0f);
  ImGui::DragFloat("Gravity", &m_gravity, 0.05f, 0.05f, 20.0f);
  ImGui::DragFloat("Rotation", &m_rotation_deg, 1.f, -180.f, 0.0f);
}

void Player::OnRendererCall()
{
  Pepper::Renderer2D::DrawRotatedQuad(m_position, m_rocket_size, m_rotation_deg, m_rocket_tex);
  Pepper::Renderer2D::DrawRotatedQuad({ m_position.x, m_position.y, 0.1 },
                                      m_flames_size,
                                      m_rotation_deg,
                                      m_flame_tex,
                                      1.0f,
                                      { 1.0f, 1.0f, 1.0f, m_flame_alpha });
}
