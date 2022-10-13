#if defined(PROFILING)

// clang-format off
#include "PepperPCH.hpp"
// clang-format on

  #include "Pepper/Profiling/Timer.hpp"

  #include "Pepper/Profiling/Profiler.hpp"
  #include "Pepper/Profiling/ProfilerData.hpp"

namespace Pepper
{
  static std::mutex _s_mutex;

  static uint64_t _s_last_time = 0;
  auto GetNowTime()
  {
    std::chrono::time_point<std::chrono::high_resolution_clock> instant;
    uint64_t instant_us{};

    instant = std::chrono::high_resolution_clock::now();
    instant_us = std::chrono::time_point_cast<std::chrono::microseconds>(instant).time_since_epoch().count();
    if (instant_us == _s_last_time)
      instant_us += 1;

    _s_last_time = instant_us;
    return instant_us;
  }

  class Timer::Impl
  {
  public:
    Impl(std::string&& title);

    std::string m_title;
    uint64_t m_start_point;
    bool m_stopped;
  };

  Timer::Impl::Impl(std::string&& title) : m_title(title), m_start_point(), m_stopped(false)
  {
    if (!Profiler::Get().IsActive())
      return;

    std::lock_guard<std::mutex> lock{ _s_mutex };
    m_start_point = GetNowTime();
  }

  Timer::Timer(std::string&& title) : pimp(CreateScope<Impl>(std::move(title))) {}

  Timer::~Timer()
  {
    if (!pimp->m_stopped)
      Stop();
  }

  void Timer::Stop()
  {
    if (!Profiler::Get().IsActive())
      return;

    auto end_time = GetNowTime();

    uint64_t thread_hash = std::hash<std::thread::id>{}(std::this_thread::get_id());
    int thread_num = Profiler::Get().GetThreadNum(thread_hash);

    Scope<ProfilerData> data = CreateScope<ProfilerData>(std::move(pimp->m_title), pimp->m_start_point, end_time, thread_num);
    Profiler::Get().EnqueueTimer(std::move(data));

    pimp->m_stopped = true;
  }

  bool Timer::operator<(const Timer& other) const
  {
    return this->pimp->m_start_point < other.pimp->m_start_point;
  }

}
#endif
