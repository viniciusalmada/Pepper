// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Level.hpp"

#include "Color.hpp"

#include <imgui.h>

namespace
{
  const float Y_LOWER_LIMIT = -5.0f;
  const float Y_UPPER_LIMIT = +5.0f;
}

Level::Level() {}

Level::~Level()
{
  finish = true;
  m_obstacles_updater.join();
}

void Level::Init()
{
  PP_PROFILE_FUNCTION();
  m_player.LoadAssets();

  for (auto& obs : m_obstacles)
  {
    UpdateObstacle(obs);
  }

  m_obstacles_updater = std::jthread{ std::bind(&Level::CheckObstacles, this) };
}

void Level::OnUpdate(Pepper::TimeStep ts)
{
  PP_PROFILE_FUNCTION();
  m_player.OnUpdate(ts);
  do_update = true;
}

void Level::OnImGuiRender()
{
  PP_PROFILE_FUNCTION();
  m_player.OnImGuiLayer();
}

void Level::OnRendererCall()
{
  PP_PROFILE_FUNCTION();
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
  PP_PROFILE_FUNCTION();
  return m_player.GetPosition();
}

void Level::CheckObstacles()
{
  PP_PROFILE_FUNCTION();
  while (!finish)
  {
    if (!do_update)
      continue;

    for (auto& obs : m_obstacles)
    {
      if (m_player.GetPosition().x - 15.0f > obs.m_position.x)
      {
        UpdateObstacle(obs);
      }
    }
    do_update = false;
  }
}

void Level::UpdateObstacle(Obstacle& obs)
{
  PP_PROFILE_FUNCTION();
  obs.m_position.x = m_obs_x_pos;
  obs.m_size.y = static_cast<float>(rand() % 6);
  obs.m_size.x = obs.m_size.y * 2;
  PP_TRACE("Obs height = {0}", obs.m_size.y);
  if (rand() % 2 == 0) // random - down
  {
    obs.m_position.y = Y_LOWER_LIMIT + obs.m_size.y / 2.0f;
  }
  else
  {
    obs.m_position.y = Y_UPPER_LIMIT - obs.m_size.y / 2.0f;
  }
  obs.m_position.z = 0.15f;

  m_obs_x_pos += 10.f;
}
