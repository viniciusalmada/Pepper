#pragma once

#include <glm/glm.hpp>
namespace Utils
{
  float Interpol(float srcMin, float srcMax, float tarMin, float tarMax, float ref);

  bool LineIntersectCircle(const glm::vec2& p1, const glm::vec2& p2, float radius, const glm::vec2& center);
}