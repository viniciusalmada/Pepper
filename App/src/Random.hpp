#pragma once

#include <random>

class Random
{
public:
  static void Init();

  static int32_t Int(int32_t min = std::numeric_limits<int32_t>::min(),
                     int32_t max = std::numeric_limits<int32_t>::max());

  static float Float(float min = std::numeric_limits<float>::min(),
                     float max = std::numeric_limits<float>::max());

private:
  static std::mt19937 s_random_engine;
};