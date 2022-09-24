#include "Random.hpp"

std::mt19937 Random::s_random_engine{};

void Random::Init()
{
  PP_INFO("{0}", std::random_device{}());
  s_random_engine.seed(std::random_device{}());
}

int32_t Random::Int(int32_t min, int32_t max)
{
  std::uniform_int_distribution<int32_t> distribution{min, max};

  return distribution(s_random_engine);
};

float Random::Float(float min, float max)
{
   std::uniform_real_distribution<float> distribution{min, max};

  return distribution(s_random_engine);
}
