// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Level.hpp"

#include "Color.hpp"
#include "Random.hpp"
#include "Utilities.hpp"

#include <imgui.h>

namespace
{
  const float Y_LOWER_LIMIT = -5.0f;
  const float Y_UPPER_LIMIT = +5.0f;
}

Level::Level() {}

Level::~Level()
{
  m_is_shutdown = true;
  m_planets_updater.join();
  m_collision_detector.join();
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

  for (auto& planet : m_planets)
  {
    UpdatePlanet(planet);
  }

  m_planets_updater = std::jthread{ std::bind(&Level::CheckPlanetsPosition, this) };
  m_collision_detector = std::jthread{ std::bind(&Level::CheckCollision, this) };
}

void Level::OnUpdate(Pepper::TimeStep ts)
{
  PP_PROFILE_FUNCTION();
  m_player.OnUpdate(ts);
  m_is_planet_passed = true;
}

void Level::OnImGuiRender()
{
  PP_PROFILE_FUNCTION();
  m_player.OnImGuiLayer();
}

void Level::OnRendererCall()
{
  PP_PROFILE_FUNCTION();

  m_player.OnRendererCall();

  for (const auto& planet : m_planets)
  {
    planet.OnRendererCall();
  }
}

const glm::vec3& Level::GetPlayerPosition() const
{
  PP_PROFILE_FUNCTION();
  return m_player.GetPosition();
}

void Level::CheckPlanetsPosition()
{
  PP_PROFILE_FUNCTION();
  while (!m_is_shutdown)
  {
    if (!m_is_planet_passed)
      continue;

    for (auto& planet : m_planets)
    {
      if (m_player.GetPosition().x - 15.0f > planet.GetPosition().x)
      {
        UpdatePlanet(planet);
        m_is_planet_passed = true;
        break;
      }
    }
  }
}
void Level::UpdatePlanet(Planet& planet)
{
  PP_PROFILE_FUNCTION();
  planet.Update(m_planet_new_position, m_planets_textures[Random::Int(0, 7)]);

  m_planet_new_position += 10.f;
}

void Level::CheckCollision()
{
  while (!m_is_shutdown)
  {
    const auto& player_bbox = m_player.GetBoundingBox();
    bool player_touched = false;
    for (const auto& planet : m_planets)
    {
      float radius = planet.GetRadius();
      const glm::vec3& center = planet.GetPosition();

      for (const auto& corner : player_bbox)
      {
        float dist = glm::distance(glm::vec3{ corner }, center);
        if (dist <= radius)
        {
          player_touched = true;
          break;
        }
      }

      if (player_touched)
      {
        PP_WARN("Player touched! - corner touched");
        GameOver();
        break;
      }

      for (auto i : std::ranges::iota_view{ 0u, player_bbox.size() })
      {
        uint32_t j = i + 1 == player_bbox.size() ? 0 : i + 1;
        const glm::vec2 p1{ player_bbox[i] };
        const glm::vec2 p2{ player_bbox[j] };

        if (Utils::LineIntersectCircle(p1, p2, radius, glm::vec2{ planet.GetPosition() }))
        {
          player_touched = true;
          break;
        }
      }

      if (player_touched)
      {
        PP_WARN("Player touched! - line touched");
        GameOver();
        break;
      }
    }
  }
}
