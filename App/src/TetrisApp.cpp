#include "TetrisApp.hpp"

#include "Constants.hpp"

#include <Pepper/Core/EntryPoint.hpp>

TetrisApp::TetrisApp() :
    m_game_layer(Pepper::CreateRef<GameLayer>()),
    Application("Tetris", WINDOW_WIDTH, WINDOW_HEIGHT)
{
  PushLayer(m_game_layer);
}

Pepper::Scope<Pepper::Application> Pepper::CreateApplication()
{
  return CreateScope<TetrisApp>();
}