#pragma once

#include <Pepper.hpp>

namespace
{
  constexpr auto WINDOW_WIDTH = 480;
  constexpr auto WINDOW_HEIGHT = 640;

  const Pepper::OrthoCamera CAMERA{
    {0, WINDOW_WIDTH, 0, WINDOW_HEIGHT}
  };
}