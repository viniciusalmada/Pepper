#pragma once

#include <glm/glm.hpp>

#include <cstdlib>

namespace Color
{
  constexpr glm::vec4 BLACK{ 0.0f, 0.0f, 0.0f, 1.0f };
  constexpr glm::vec4 RED{ 1.0f, 0.0f, 0.0f, 1.0f };

  const glm::vec4 RandColor()
  {
    int red = rand() % 255;
    int green = rand() % 255;
    int blue = rand() % 255;

    return glm::vec4{ red / 255.0f, green / 255.0f, blue / 255.0f, 1.0f };
  }
}