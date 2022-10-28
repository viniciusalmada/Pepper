// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Level.hpp"

#include "Color.hpp"
#include "Random.hpp"
#include "Utilities.hpp"

#include <imgui.h>
#include <utility>

Level::Level() = default;

Level::~Level() = default;

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
  m_next_planet = Pepper::CreateRef<Planet>(m_planets[0]);
}

void Level::OnUpdate(Pepper::TimeStep ts)
{
  PP_PROFILE_FUNCTION()

  m_player.OnUpdate(ts);

  if (m_player.GetPosition().x > m_next_planet->GetPosition().x + m_next_planet->GetRadius())
  {
    for (auto i : std::views::iota(0u, m_planets.size()))
    {
      if (m_planets[i] == *m_next_planet.get())
      {
        m_score++;
        PP_INFO("Earned 1 point: SCORE={0}", m_score);
        m_next_planet = Pepper::CreateRef<Planet>(m_planets[i + 1 >= m_planets.size() ? 0 : i + 1]);
        break;
      }
    }
  }

  for (auto& planet : m_planets)
  {
    if (m_player.GetPosition().x - 15.0f > planet.GetPosition().x)
    {
      UpdatePlanet(planet);
      break;
    }
  }

  const auto& player_bbox = m_player.GetBoundingBox();
  bool player_touched = false;
  for (const auto& planet : m_planets)
  {
    for (auto i : std::ranges::iota_view{ 0u, player_bbox.size() })
    {
      uint32_t j = i + 1 == player_bbox.size() ? 0 : i + 1;
      const glm::vec2 p1{ player_bbox[i] };
      const glm::vec2 p2{ player_bbox[j] };

      auto [touched, _] =
        Utils::LineIntersectCircle(p1, p2, planet.GetRadius(), glm::vec2{ planet.GetPosition() });
      if (touched)
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

void Level::UpdatePlanet(Planet& planet)
{
  PP_PROFILE_FUNCTION()
  auto plan_tex_id = Random::Int(0, 7);
  PP_INFO("Next planet: {0}", plan_tex_id);
  planet.Update(m_planet_new_position, m_planets_textures[plan_tex_id]);

  m_planet_new_position += 10.f;
}

void Level::GameOver()
{
  PP_INFO("New round starting!");
  m_player.Reset();

  m_planet_new_position = 20.0f;

  m_planets = std::array<Planet, 3>{};
  for (auto& planet : m_planets)
  {
    UpdatePlanet(planet);
  }
  m_next_planet = Pepper::CreateRef<Planet>(m_planets[0]);

  m_on_game_over_cb(m_score);
  m_score = 0;
}

uint32_t Level::GetPlayerScore() const
{
  return this->m_score;
}

void Level::Restart()
{
  m_player.StartMoving();
}
void Level::SetGameOverCallback(std::function<void(uint32_t)> onGameOver)
{
  m_on_game_over_cb = onGameOver;
}
