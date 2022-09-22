// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Level.hpp"

Level::Level() {}

void Level::Init()
{
  m_player.LoadAssets();
}

void Level::OnUpdate(Pepper::TimeStep ts)
{
  m_player.OnUpdate(ts);
}

void Level::OnImGuiRender()
{
  m_player.OnImGuiLayer();
}

void Level::OnRendererCall()
{
  m_player.OnRendererCall();
}
