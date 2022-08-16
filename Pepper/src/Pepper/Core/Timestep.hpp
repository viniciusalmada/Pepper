#pragma once

#include <cstdint>

namespace Pepper
{
  class Timestep
  {
  public:
    Timestep(float time) : time_sec(time) {}

    operator float() const { return time_sec; }

    float GetSeconds() const { return time_sec; }

    float GetMilliSeconds() const { return time_sec * 1000.0f; }

  private:
    float time_sec;
  };
}
