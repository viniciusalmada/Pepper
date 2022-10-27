#include "Random.hpp"

namespace
{
  std::mt19937 s_random_engine{ std::random_device{}() };
}

std::uniform_int_distribution<int32_t> Random::s_int32_distribution{};
std::uniform_real_distribution<float> Random::s_float_distribution{};

using IntParam = std::uniform_int_distribution<int32_t>::param_type;
using FloatParam = std::uniform_real_distribution<float>::param_type;

int32_t Random::Int(int32_t min, int32_t max)
{
  s_int32_distribution.param(IntParam(min, max));

  return s_int32_distribution(s_random_engine);
}

float Random::Float(float min, float max)
{
  s_float_distribution.param(FloatParam(min, max));

  return s_float_distribution(s_random_engine);
}
