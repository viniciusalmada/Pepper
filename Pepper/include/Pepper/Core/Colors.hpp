#pragma once

#include <glm/glm.hpp>

namespace Color
{
  constexpr auto BLACK = glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f };
  constexpr auto WHITE = glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f };

  static glm::vec4
  FromRGB(uint8_t r, uint8_t g, uint8_t b, uint8_t a = std::numeric_limits<uint8_t>::max())
  {
    float max = std::numeric_limits<uint8_t>::max();
    return { static_cast<float>(r) / max,
             static_cast<float>(g) / max,
             static_cast<float>(b) / max,
             static_cast<float>(a) / max };
  }
}