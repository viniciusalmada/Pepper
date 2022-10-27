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

  [[nodiscard]] const glm::vec3& GetPlayerPosition() const;

  [[nodiscard]] uint32_t GetPlayerScore() const;

private:
  void UpdatePlanet(Planet& obs);

  void GameOver();

  Player m_player;
  uint32_t m_score = 0;
  Pepper::Ref<Planet> m_next_planet;
  std::array<Planet, 3> m_planets;
  float m_planet_new_position = 5.0f;

  std::array<Pepper::Ref<Pepper::Texture2D>, 8> m_planets_textures;
};