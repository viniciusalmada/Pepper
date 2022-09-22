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
  }
  else
  {
    m_velocity.y -= m_gravity;
  }
  m_velocity.y = glm::clamp(m_velocity.y, -20.0f, 20.0f);
  m_position += m_velocity * (float)ts;
}

void Player::OnImGuiLayer()
{
  ImGui::DragFloat("Engine Power", &m_engine_power, 0.1f);
  ImGui::DragFloat("Gravity", &m_gravity, 0.1f);
}

void Player::OnRendererCall()
{
  Pepper::Renderer2D::DrawQuad(m_position, { 1.0f, 1.875f }, m_texture);
}
