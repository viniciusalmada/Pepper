// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Pepper/Utilities/Random.hpp"

namespace
{
  std::default_random_engine g_random_engine{ std::random_device{}() };
}

namespace Pepper
{
  using IntParam = std::uniform_int_distribution<int32_t>::param_type;
  using FloatParam = std::uniform_real_distribution<float>::param_type;

  int32_t IntRandom(int32_t minValue, int32_t maxValue)
  {
    static std::uniform_int_distribution<int32_t> distribution{};
    distribution.param(IntParam(minValue, maxValue));
    return distribution(g_random_engine);
  }

  float FloatRandom(float minValue, float maxValue)
  {
    static std::uniform_real_distribution<float> distribution{};
    distribution.param(FloatParam(minValue, maxValue));
    return distribution(g_random_engine);
  }
} // Pepper