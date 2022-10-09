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
std::tuple<bool, glm::vec2>
Utils::LineIntersectCircle(const glm::vec2& p1, const glm::vec2& p2, float radius, const glm::vec2& center)
{
  if (p1 == p2)
    return { false, {} };

  const float dist_p1 = glm::distance(p1, center);
  if (dist_p1 <= radius)
    return { true, p1 };
  const float dist_p2 = glm::distance(p2, center);
  if (dist_p2 <= radius)
    return { true, p2 };

  glm::vec2 new_p1 = p1 - center;
  glm::vec2 new_p2 = p2 - center;

  glm::vec2 d = new_p2 - new_p1;
  float d_r = glm::length(d);
  float d_big = new_p1.x * new_p2.y - new_p2.x * new_p1.y;

  float discriminant = std::pow(radius, 2.0f) * std::pow(d_r, 2.0f) - std::pow(d_big, 2.0f);
  if (discriminant < 0.0f)
    return { false, {} };

  auto sig = [](float x)
  { return x < 0.0f ? -1.0f : 1.0f; };

  if (dist_p1 <= radius || dist_p2 <= radius)
  {
    glm::vec2 s_intersection_pt;
    s_intersection_pt.x = (d_big * d.y + sig(d.y) * d.x * std::sqrtf(discriminant)) / (d_r * d_r);
    s_intersection_pt.y = (-d_big * d.x + std::abs(d.y) * std::sqrtf(discriminant)) / (d_r * d_r);

    float dist_1 = glm::distance(s_intersection_pt, new_p1);
    float dist_2 = glm::distance(s_intersection_pt, new_p2);

    if (dist_1 + dist_2 <= d_r)
    {
      s_intersection_pt += center;
      return { true, s_intersection_pt };
    }
  }

  return { false, {} };
}