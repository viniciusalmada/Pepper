#if defined(PROFILING)

  #pragma once

  #include <cstdint>
  #include <string>

namespace Pepper
{
  struct ProfilerData
  {
    std::string m_title;
    uint64_t m_start_us;
    uint64_t m_end_us;
    uint64_t m_thread_hash;
  };
}
#endif