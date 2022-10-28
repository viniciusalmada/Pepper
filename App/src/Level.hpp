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

  void Restart();

  void SetGameOverCallback(std::function<void(uint32_t)> onGameOver);

private:
  void UpdatePlanet(Planet& obs);

  void GameOver();

  std::function<void(uint32_t)> m_on_game_over_cb;
  Player m_player;
  uint32_t m_score = 0;
  Pepper::Ref<Planet> m_next_planet;
  std::array<Planet, 3> m_planets;
  float m_planet_new_position = 20.0f;

  std::array<Pepper::Ref<Pepper::Texture2D>, 8> m_planets_textures;
};