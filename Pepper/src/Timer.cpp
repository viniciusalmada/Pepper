// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Pepper/Profiling/Timer.hpp"

#include "Pepper/Profiling/Profiler.hpp"
#include "Pepper/Profiling/ProfilerData.hpp"

namespace Pepper
{
  auto GetNowTime()
  {
    return std::chrono::high_resolution_clock::now();
  }

  class Timer::Impl
  {
  public:
    Impl(std::string&& title);

    std::string m_title;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start_point;
    bool m_stopped;
  };

  Timer::Impl::Impl(std::string&& title) : m_title(title), m_start_point(GetNowTime()), m_stopped(false) {}

  Timer::Timer(std::string&& title) : pimp(CreateScope<Impl>(std::move(title))) {}

  Timer::~Timer()
  {
    if (!pimp->m_stopped)
      Stop();
  }

  void Timer::Stop()
  {
    auto end_time = GetNowTime();

    uint64_t start =
      std::chrono::time_point_cast<std::chrono::nanoseconds>(pimp->m_start_point).time_since_epoch().count();
    uint64_t end = std::chrono::time_point_cast<std::chrono::nanoseconds>(end_time).time_since_epoch().count();

    uint64_t thread_hash = std::hash<std::thread::id>{}(std::this_thread::get_id());

    Profiler::Get().WriteTimer(ProfilerData{ std::move(pimp->m_title), start, end, thread_hash });

    pimp->m_stopped = true;
  }

}