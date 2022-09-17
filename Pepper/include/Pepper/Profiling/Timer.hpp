#if defined(PROFILING)
  #pragma once

  #include "Pepper/Core/Core.hpp"

namespace Pepper
{
  class Timer
  {
  public:
    Timer(std::string&& title);
    ~Timer();

    void Stop();

  private:
    DECLARE_PIMPL
  };
}
#endif