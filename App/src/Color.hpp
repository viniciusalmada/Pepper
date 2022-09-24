#pragma once

#include <glm/glm.hpp>

#include "Random.hpp"

namespace Color
{
  constexpr glm::vec4 BLACK{ 0.0f, 0.0f, 0.0f, 1.0f };
  constexpr glm::vec4 RED{ 1.0f, 0.0f, 0.0f, 1.0f };

  const glm::vec4 RandColor()
  {
    int red = Random::Int(0, 255);
    int green = Random::Int(0, 255);
    int blue = Random::Int(0, 255);

    return glm::vec4{ red / 255.0f, green / 255.0f, blue / 255.0f, 1.0f };
  }
}