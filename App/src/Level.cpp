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

  m_planets_textures[0] = Pepper::Texture2D::Create("assets/textures/plan-earth.png");
  m_planets_textures[1] = Pepper::Texture2D::Create("assets/textures/plan-jupiter.png");
  m_planets_textures[2] = Pepper::Texture2D::Create("assets/textures/plan-mars.png");
  m_planets_textures[3] = Pepper::Texture2D::Create("assets/textures/plan-mercury.png");
  m_planets_textures[4] = Pepper::Texture2D::Create("assets/textures/plan-moon-phase.png");
  m_planets_textures[5] = Pepper::Texture2D::Create("assets/textures/plan-neptune.png");
  m_planets_textures[6] = Pepper::Texture2D::Create("assets/textures/plan-pluto.png");
  m_planets_textures[7] = Pepper::Texture2D::Create("assets/textures/plan-uranus.png");

  for (auto& obs : m_obstacles)
  {
    obs.m_color = Color::RandColor();
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
    if (obs.m_is_top)
      Pepper::Renderer2D::DrawPixelateQuad(obs.m_position, obs.m_size, obs.m_texture, obs.m_size.x * 10.0f);
    else
      Pepper::Renderer2D::DrawPixelateQuad(obs.m_position,
                                           { obs.m_size.x, -obs.m_size.y },
                                           obs.m_texture,
                                           obs.m_size.x * 10.0f);
  }

  m_player.OnRendererCall();
}

const glm::vec3& Level::GetPlayerPosition() const
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
  obs.m_texture = m_planets_textures[Random::Int(0, 7)];
  obs.m_size.y = Random::Float(1.0f, 10.0f);
  obs.m_size.x = obs.m_size.y;
  PP_TRACE("Obs height = {0}", obs.m_size.y);
  if (Random::Int(0, 1) % 2 == 0) // random - down
  {
    obs.m_position.y = Y_LOWER_LIMIT;
    obs.m_is_top = true;
  }
  else
  {
    obs.m_position.y = Y_UPPER_LIMIT;
    obs.m_is_top = false;
  }
  obs.m_position.z = 0.15f;
  obs.m_color = Color::RandColor();

  m_obs_x_pos += 10.f;
}
