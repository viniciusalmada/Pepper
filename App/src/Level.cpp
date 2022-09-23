// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Level.hpp"

#include "Color.hpp"

Level::Level() {}

void Level::Init()
{
  m_player.LoadAssets();
}

void Level::OnUpdate(Pepper::TimeStep ts)
{
  m_player.OnUpdate(ts);
}

void Level::OnImGuiRender()
{
  m_player.OnImGuiLayer();
}

void Level::OnRendererCall()
{
  const auto& player_pos = m_player.GetPosition();

  Pepper::Renderer2D::DrawQuad({ player_pos.x, -5.25 }, { 20.0, 0.5 }, Color::BLACK);
  Pepper::Renderer2D::DrawQuad({ player_pos.x, +5.25 }, { 20.0, 0.5 }, Color::BLACK);

  m_player.OnRendererCall();
}

const glm::vec2& Level::GetPlayerPosition() const
{
  return m_player.GetPosition();
}
