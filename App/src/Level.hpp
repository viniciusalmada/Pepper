#pragma once

#include "Planet.hpp"
#include "Player.hpp"

#include <Pepper.hpp>

class Level
{
public:
  Level();
  ~Level();

  void Init();

  void OnUpdate(Pepper::TimeStep);

  void OnImGuiRender();

  void OnRendererCall();

  const glm::vec3& GetPlayerPosition() const;

private:
  void CheckPlanetsPosition();

  void UpdatePlanet(Planet& obs);

  void CheckCollision();

  void GameOver();

  Player m_player;
  std::array<Planet, 3> m_planets;
  float m_planet_new_position = 5.0f;
  bool m_is_shutdown = false;

  std::jthread m_planets_updater;
  std::jthread m_collision_detector;

  std::array<Pepper::Ref<Pepper::Texture2D>, 8> m_planets_textures;
};