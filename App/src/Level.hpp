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

private:
  Player m_player;
};