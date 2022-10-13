// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Level.hpp"

#include "Color.hpp"
#include "Random.hpp"
#include "Utilities.hpp"

#include <imgui.h>

Level::Level() = default;

Level::~Level()
{
  m_is_shutdown = true;
  if (m_planets_updater.joinable())
    m_planets_updater.join();
  if (m_collision_detector.joinable())
    m_collision_detector.join();
}

void Level::Init()
{
  PP_PROFILE_FUNCTION()
  m_player.Init();

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
}

void Level::OnUpdate(Pepper::TimeStep ts)
{
  PP_PROFILE_FUNCTION()

  if (!m_collision_detector.joinable())
    m_collision_detector = std::jthread([this] { CheckCollision(); });

  if (!m_planets_updater.joinable())
    m_planets_updater = std::jthread([this] { CheckPlanetsPosition(); });

  m_player.OnUpdate(ts);

  //  CollisionChecker();
  //  PlanetsPositionChecker();
}

void Level::OnImGuiRender()
{
  PP_PROFILE_FUNCTION()
  if (ImGui::Button("Restart"))
  {
    GameOver();
    return;
  }
  m_player.OnImGuiLayer();
}

void Level::OnRendererCall()
{
  PP_PROFILE_FUNCTION()
  for (const auto& planet : m_planets)
  {
    planet.OnRendererCall();
  }

  m_player.OnRendererCall();
}

const glm::vec3& Level::GetPlayerPosition() const
{
  PP_PROFILE_FUNCTION()
  return m_player.GetPosition();
}

void Level::CheckPlanetsPosition()
{
  PP_PROFILE_FUNCTION()
  while (!m_is_shutdown)
  {
    PlanetsPositionChecker();
  }
}

void Level::UpdatePlanet(Planet& planet)
{
  PP_PROFILE_FUNCTION()
  planet.Update(m_planet_new_position, m_planets_textures[Random::Int(0, 7)]);

  m_planet_new_position += 10.f;
}

void Level::CheckCollision()
{
  while (!m_is_shutdown)
  {
    CollisionChecker();
  }
}

void Level::GameOver()
{
  m_player.Reset();

  m_planet_new_position = 5.0f;
  m_is_shutdown = false;

  m_planets = std::array<Planet, 3>{};
  for (auto& planet : m_planets)
  {
    UpdatePlanet(planet);
  }
}

void Level::CollisionChecker()
{
  const auto& player_bbox = m_player.GetBoundingBox();
  bool player_touched = false;
  for (const auto& planet : m_planets)
  {
    for (auto i : std::ranges::iota_view{ 0u, player_bbox.size() })
    {
      uint32_t j = i + 1 == player_bbox.size() ? 0 : i + 1;
      const glm::vec2 p1{ player_bbox[i] };
      const glm::vec2 p2{ player_bbox[j] };

      auto [touched, _] = Utils::LineIntersectCircle(p1, p2, planet.GetRadius(), glm::vec2{ planet.GetPosition() });
      if (touched)
      {
        player_touched = true;
        PP_WARN("p1=({0},{1}), p2=({2},{3}), r={4}, center=({5},{6})",
                p1.x,
                p1.y,
                p2.x,
                p2.y,
                planet.GetRadius(),
                planet.GetPosition().x,
                planet.GetPosition().y);
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
void Level::PlanetsPositionChecker()
{
  for (auto& planet : m_planets)
  {
    if (m_player.GetPosition().x - 15.0f > planet.GetPosition().x)
    {
      UpdatePlanet(planet);
      break;
    }
  }
}
