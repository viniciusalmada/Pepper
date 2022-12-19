#pragma once

#include <Pepper.hpp>

namespace
{
  constexpr auto WINDOW_WIDTH = 480;
  constexpr auto WINDOW_HEIGHT = 624;

  constexpr auto LAYER_BG = 0.1f;
  constexpr auto LAYER_PANEL = 0.2f;

  constexpr auto QUAD_SIDE = 28.0f;
  constexpr auto CELL_SIDE = 30.0f;
  constexpr auto SCENE_HEIGHT = 20 * CELL_SIDE + CELL_SIDE - QUAD_SIDE;
  constexpr auto SCENE_WIDTH = 10 * CELL_SIDE + CELL_SIDE - QUAD_SIDE;
  constexpr auto HALF_QUAD_SIDE = QUAD_SIDE / 2.0f;
  constexpr auto HALF_CELL_SIDE = QUAD_SIDE / 2.0f;

  constexpr auto RED = glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f };
  constexpr auto GREY = glm::vec4{ 0.7f, 0.7f, 0.7f, 0.75f };

  const Pepper::OrthoCamera CAMERA{
    Pepper::CameraLimits{0, WINDOW_WIDTH, WINDOW_HEIGHT, 0}
  };
}