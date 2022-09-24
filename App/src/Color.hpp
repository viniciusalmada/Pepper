#pragma once

#include <glm/glm.hpp>

namespace Color
{
  constexpr glm::vec4 BLACK{ 0.0f, 0.0f, 0.0f, 1.0f };
  constexpr glm::vec4 RED{ 1.0f, 0.0f, 0.0f, 1.0f };

  const glm::vec4 RandColor();
}