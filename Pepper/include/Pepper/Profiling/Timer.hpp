#if defined(PROFILING)
  #pragma once

  #include "Pepper/Core/Core.hpp"

namespace Pepper
{
  class Timer
  {
  public:
    explicit Timer(std::string&& title);
    ~Timer();
    void Stop();

    bool operator<(const Timer& other) const;

  private:
    DECLARE_PIMPL
  };
}
#endif