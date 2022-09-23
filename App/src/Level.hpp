#pragma once

#include "Player.hpp"

struct Obstacle
{
  glm::vec3 m_position{ 0.0f, 0.0f, 0.0f };
  glm::vec2 m_size{ 1.0, 4.0f };
};

class Level
{
public:
  Level();

  void Init();

  void OnUpdate(Pepper::TimeStep);

  void OnImGuiRender();

  void OnRendererCall();

  const glm::vec2& GetPlayerPosition() const;

private:
  void UpdateObstacle(Obstacle& obs);

  Player m_player;
  std::array<Obstacle, 5> m_obstacles;
  float m_obs_x_pos = 5.0f;

  static const float OBSTACLE_HIDDEN;
};