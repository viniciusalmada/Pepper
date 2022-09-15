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

    void Stop();

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
      pimp->Stop();
  }

  void Timer::Impl::Stop()
  {
    auto end_time_point = std::chrono::steady_clock::now();

    uint64_t start =
      std::chrono::time_point_cast<std::chrono::milliseconds>(start_time_point).time_since_epoch().count();
    uint64_t end = std::chrono::time_point_cast<std::chrono::milliseconds>(end_time_point).time_since_epoch().count();

    stopped = true;

    PP_CORE_TRACE("Duration of {0}: {1}ms", name, end - start);
  }
}