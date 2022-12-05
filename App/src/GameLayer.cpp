#include "GameLayer.hpp"

#include "Constants.hpp"

using namespace Pepper;

struct GameLayer::Implementation
{
  OrthoCameraController camera_ctrl{ (float)WINDOW_HEIGHT / (float)WINDOW_WIDTH };
};

GameLayer::GameLayer() : Layer("GameLayer"), impl(CreateScope<Implementation>()) {}

GameLayer::~GameLayer() = default;

void GameLayer::OnAttach()
{
  PP_INFO("GameLayer attached!");
}

void GameLayer::OnUpdate(Pepper::TimeStep ts)
{
  impl->camera_ctrl.OnUpdate(ts);
  Pepper::RenderCommand::SetClearColor(Color::BLACK);
  Pepper::RenderCommand::Clear();
}
