#pragma once

#include "Player.hpp"

#include <Pepper.hpp>

struct Obstacle
{
  glm::vec3 m_position{ 0.0f, 0.0f, 0.0f };
  glm::vec2 m_size{ 1.0, 10.0f };
  Pepper::Ref<Pepper::Texture2D> m_texture;
  glm::vec4 m_color;
  bool m_is_top = true;
};

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
  void CheckObstacles();

  void UpdateObstacle(Obstacle& obs);

  Player m_player;
  std::array<Obstacle, 3> m_obstacles;
  float m_obs_x_pos = 5.0f;
  std::jthread m_obstacles_updater;
  bool do_update = false;
  bool finish = false;

  std::array<Pepper::Ref<Pepper::Texture2D>, 8> m_planets_textures;
};