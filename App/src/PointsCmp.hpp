//
// Created by vinic on 19-Oct-22.
//

#ifndef PEPPER_POINTSCMP_HPP
#define PEPPER_POINTSCMP_HPP

#include "glm/vec2.hpp"

struct PointsCmp
{
  bool operator()(const glm::dvec2& lhs, const glm::dvec2 rhs) const
  {
    if (lhs.x < rhs.x)
      return true;

    if (lhs.x == rhs.x)
    {
      if (lhs.y < rhs.y)
        return true;
    }
    return false;
  }
};

#endif // PEPPER_POINTSCMP_HPP
