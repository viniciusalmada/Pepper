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
    Impl(const std::string& name);

    std::string name;
    std::chrono::time_point<std::chrono::steady_clock> start_time_point;
    bool stopped;
  };

  Timer::Impl::Impl(const std::string& name) : name(name), stopped(false)
  {
    start_time_point = std::chrono::steady_clock::now();
  }

  Timer::Timer(const std::string& name) : pimp(CreateScope<Impl>(name)) {}

  Timer::~Timer()
  {
    if (!pimp->stopped)
      Stop();
  }

  void Timer::Stop()
  {
    auto end_time_point = std::chrono::steady_clock::now();
    pimp->stopped = true;

    auto time_spent = std::chrono::duration_cast<std::chrono::microseconds>(end_time_point - pimp->start_time_point);

    PP_CORE_TRACE("Duration of {0}: {1}ms", pimp->name, time_spent.count() / 1000.0f);
  }
}