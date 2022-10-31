// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Pepper/Core/Utils.hpp"

namespace Pepper::Utils
{
  std::string GetNewLine()
  {
#ifdef PP_PLATFORM_WINDOWS
    return "\r\n";
#else
    return "\n";
#endif
  }

  class Timer::Impl
  {
  public:
    Impl(std::string&& name, TimerFun&& fun);

    std::string name;
    std::chrono::time_point<std::chrono::steady_clock> start_time_point;
    bool stopped;
    TimerFun fun;
  };

  Timer::Impl::Impl(std::string&& name, TimerFun&& fun) : name(name), stopped(false), fun(fun)
  {
    start_time_point = std::chrono::steady_clock::now();
  }

  Timer::Timer(std::string&& name, TimerFun&& fun) :
      pimp(CreateScope<Impl>(std::move(name), std::move(fun)))
  {
  }

  Timer::~Timer()
  {
    if (!pimp->stopped)
      Stop();
  }

  void Timer::Stop()
  {
    auto end_time_point = std::chrono::steady_clock::now();
    pimp->stopped = true;

    auto time_spent =
      std::chrono::duration_cast<std::chrono::nanoseconds>(end_time_point - pimp->start_time_point);
    float time_spent_ms = time_spent.count() / 1000.0f / 1000.0f;

    pimp->fun(std::move(pimp->name), time_spent_ms);
  }
}