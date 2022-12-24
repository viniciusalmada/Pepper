#pragma once

#include <limits>
#include <random>

namespace Pepper
{
  int32_t IntRandom(int32_t minValue = std::numeric_limits<int32_t>::min(),
                    int32_t maxValue = std::numeric_limits<int32_t>::max());

  float FloatRandom(float minValue = std::numeric_limits<float>::min(),
                    float maxValue = std::numeric_limits<float>::max());

} // Pepper
