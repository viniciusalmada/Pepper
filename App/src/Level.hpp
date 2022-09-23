#pragma once

#include "Player.hpp"

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
  Player m_player;
};