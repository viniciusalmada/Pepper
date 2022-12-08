#pragma once

#include <Pepper.hpp>

namespace
{
  constexpr auto WINDOW_WIDTH = 480;
  constexpr auto WINDOW_HEIGHT = 640;

  constexpr auto QUAD_SIDE = 28.0f;
  constexpr auto HALF_QUAD_SIDE = QUAD_SIDE / 2.0f;

  constexpr auto RED = glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f };
  constexpr auto GREY = glm::vec4{ 0.7f, 0.7f, 0.7f, 0.75f };

  const Pepper::OrthoCamera CAMERA{
    {0, WINDOW_WIDTH, 0, WINDOW_HEIGHT}
  };
}