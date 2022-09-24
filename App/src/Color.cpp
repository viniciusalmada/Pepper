#include "Color.hpp"

#include "Random.hpp"

const glm::vec4 Color::RandColor()
{
  int red = Random::Int(0, 255);
  int green = Random::Int(0, 255);
  int blue = Random::Int(0, 255);

  return glm::vec4{ red / 255.0f, green / 255.0f, blue / 255.0f, 1.0f };
}
