#include "Utilities.hpp"

float Utils::Interpol(float srcMin, float srcMax, float tarMin, float tarMax, float ref)
{
  if (ref <= srcMin)
    return tarMin;

  if (ref >= srcMax)
    return tarMax;

  //  srcStep       tarStep
  // ---------- == --------------
  //  srcTotal      tarTotal
  //
  // tarStep = x - tarMin
  // x = tarStep + tarMin

  float srcTotal = srcMax - srcMin;
  float srcStep = ref - srcMin;

  float tarTotal = tarMax - tarMin;
  float tarStep = (srcStep * tarTotal) / srcTotal;

  return tarStep + tarMin;
}

/// @brief Checks if a line intersect a circle (https://mathworld.wolfram.com/Circle-LineIntersection.html)
bool Utils::LineIntersectCircle(const glm::vec2& p1, const glm::vec2& p2, float radius, const glm::vec2& center)
{
  if (p1 == p2)
    return false;

  glm::vec2 new_p1 = p1 - center;
  glm::vec2 new_p2 = p2 - center;

  glm::vec2 d = new_p2 - new_p1;
  float dr = glm::length(d);
  float d_det = new_p1.x * new_p2.y - new_p2.x * new_p1.y;

  float discriminant = std::pow(radius, 2.0f) * std::pow(dr, 2.0f) - std::pow(d_det, 2.0f);
  if (discriminant < 0.0f)
    return false;

  float dist_p1 = glm::distance(p1, center);
  float dist_p2 = glm::distance(p2, center);

  if (dist_p1 <= radius || dist_p2 <= radius)
    return true;

  return false;
}
