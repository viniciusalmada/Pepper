#pragma once

#include <glm/glm.hpp>
namespace Utils
{
  float Interpol(float srcMin, float srcMax, float tarMin, float tarMax, float ref);

  std::tuple<bool, glm::vec2> LineIntersectCircle(const glm::vec2& p1,
                                                  const glm::vec2& p2,
                                                  float radius,
                                                  const glm::vec2& center);

  void Repeat(uint64_t number, const std::function<bool()>& action);
}