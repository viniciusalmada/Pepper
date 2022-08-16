#pragma once

#include <cstdint>

namespace Pepper
{
  // The timestep entity works as a representation of the 
  // time used to render a single frame (in seconds).
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
