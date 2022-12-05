#ifndef TETRIS_APP_HPP
#define TETRIS_APP_HPP

#include "GameLayer.hpp"

#include <Pepper.hpp>

class TetrisApp : public Pepper::Application
{
public:
  TetrisApp();

private:
  Pepper::Ref<GameLayer> m_game_layer;
};

#endif // TETRIS_APP_HPP
