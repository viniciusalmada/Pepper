// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Level.hpp"

#include "Color.hpp"

Level::Level() {}

void Level::Init()
{
  m_player.LoadAssets();

  for (auto& obs : m_obstacles)
  {
    UpdateObstacle(obs);
  }
}

void Level::OnUpdate(Pepper::TimeStep ts)
{
  m_player.OnUpdate(ts);

  for (auto& obs : m_obstacles)
  {
    if (m_player.GetPosition().x - 15.0f > obs.m_position.x)
    {
      UpdateObstacle(obs);
    }
  }
}

void Level::OnImGuiRender()
{
  m_player.OnImGuiLayer();
}

void Level::OnRendererCall()
{
  const auto& player_pos = m_player.GetPosition();

  Pepper::Renderer2D::DrawQuad({ player_pos.x, -5.3125, 0.11 }, { 20.0, 0.625 }, Color::BLACK);
  Pepper::Renderer2D::DrawQuad({ player_pos.x, +5.3125, 0.11 }, { 20.0, 0.625 }, Color::BLACK);

  for (auto& obs : m_obstacles)
  {
    Pepper::Renderer2D::DrawQuad(obs.m_position, obs.m_size, Color::RED);
  }

  m_player.OnRendererCall();
}

const glm::vec2& Level::GetPlayerPosition() const
{
  return m_player.GetPosition();
}

void Level::UpdateObstacle(Obstacle& obs)
{
  obs.m_position.x = m_obs_x_pos;
  if (rand() % 2 == 0) // random
  {
    obs.m_position.y = -5.0f + obs.m_size.y / 2.0f;
  }
  else
  {
    obs.m_position.y = 5.0f - obs.m_size.y / 2.0f;
  }
  obs.m_position.z = 0.15f;

  m_obs_x_pos += 10.f;
}
