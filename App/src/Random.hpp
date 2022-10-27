#pragma once

#include <random>

class Random
{
public:
  static int32_t Int(int32_t min = std::numeric_limits<int32_t>::min(),
                     int32_t max = std::numeric_limits<int32_t>::max());

  static float Float(float min = std::numeric_limits<float>::min(),
                     float max = std::numeric_limits<float>::max());

private:
  static std::uniform_int_distribution<int32_t> s_int32_distribution;
  static std::uniform_real_distribution<float> s_float_distribution;
};
