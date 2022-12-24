// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Pepper/Utilities/Random.hpp"

namespace Pepper
{
  int32_t IntRandom(int32_t minValue, int32_t maxValue)
  {
    static std::random_device rand_dev{};
    std::mt19937 generator{ rand_dev() };
    std::uniform_int_distribution<int32_t> distribution{ minValue, maxValue };
    return distribution(generator);
  }

  float FloatRandom(float minValue, float maxValue)
  {
    static std::random_device rand_dev{};
    std::mt19937 generator{ rand_dev() };
    std::uniform_real_distribution<float> distribution{ minValue, maxValue };
    return distribution(generator);
  }
} // Pepper