#pragma once

namespace
{
  // clang-format off
  enum EvCategory : int {
    NONE         = 0,
    APPLICATION  = 0b00001,
    INPUT        = 0b00010,
    KEYBOARD     = 0b00100,
    MOUSE        = 0b01000,
    MOUSE_BUTTON = 0b10000,
  };
  // clang-format on
}