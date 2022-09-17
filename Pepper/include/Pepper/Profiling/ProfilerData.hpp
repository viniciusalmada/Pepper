#pragma once

#include <cstdint>
#include <string>

namespace Pepper
{
  struct ProfilerData
  {
    std::string m_title;
    uint64_t m_start_ns;
    uint64_t m_end_ns;
    uint64_t m_thread_hash;
  };
}