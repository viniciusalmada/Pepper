// clang-format off
#include <PepperPCH.hpp>
// clang-format on

#include "Player.hpp"

#include <imgui.h>

Player::Player() {}

void Player::LoadAssets()
{
  m_texture = Pepper::Texture2D::Create("assets/textures/rocket.png");
}

void Player::OnUpdate(Pepper::TimeStep ts)
{
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
  m_velocity.y = std::clamp(m_velocity.y, -20.0f, 20.0f);
  m_position += m_velocity * (float)ts;

  m_position.y = std::clamp(m_position.y, -5.625f - m_size.y, 5.625f + m_size.y);
}

void Player::OnImGuiLayer()
{
  ImGui::DragFloat("Engine Power", &m_engine_power, 0.05f, 0.05f, 20.0f);
  ImGui::DragFloat("Gravity", &m_gravity, 0.05f, 0.05f, 20.0f);
  ImGui::DragFloat("Rotation", &m_rotation_deg, 1.f, -180.f, 0.0f);
}

void Player::OnRendererCall()
{
  Pepper::Renderer2D::DrawRotatedQuad(m_position, m_size, m_rotation_deg, m_texture);
}
