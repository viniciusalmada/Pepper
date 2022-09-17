#pragma once

#include "Pepper/Core/Core.hpp"
#include "Pepper/Profiling/ProfilerData.hpp"
#include "Timer.hpp"

namespace Pepper
{
  class Profiler
  {
  public:
    Profiler();
    ~Profiler();

    void BeginSession(std::string&& filepath);

    void EndSession();

    void WriteTimer(ProfilerData&& data);

    static Profiler& Get();

  private:
    DECLARE_PIMPL
  };
}