#include "Color.hpp"

#include "Random.hpp"

glm::vec4 Color::RandColor()
{
  auto red = Random::Float(0.0f, 255.0f);
  auto green = Random::Float(0.0f, 255.0f);
  auto blue = Random::Float(0.0f, 255.0f);

  return glm::vec4{ red / 255.0f, green / 255.0f, blue / 255.0f, 1.0f };
}
