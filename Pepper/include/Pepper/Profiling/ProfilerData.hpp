#if defined(PROFILING)

  #pragma once

  #include "Pepper/Core/Log.hpp"

  #include <cstdint>
  #include <string>
  #include <utility>

namespace Pepper
{
  class ProfilerData
  {
  public:
    ProfilerData(std::string title, uint64_t start, uint64_t end, int threadNum);
//    ProfilerData(const ProfilerData& other);
//    ProfilerData& operator=(const ProfilerData& other);
//    ProfilerData(ProfilerData&& other) noexcept;
//    ProfilerData& operator=(ProfilerData&& other) = default;
    ~ProfilerData();

    [[nodiscard]] uint64_t GetThread() const;

    [[nodiscard]] const std::string& GetTitle() const;

    friend std::ofstream& operator<<(std::ofstream& output, const ProfilerData& data);

    std::weak_ordering operator<=>(const ProfilerData& other) const;

  private:
    std::string m_title;
    uint64_t m_start_us;
    uint64_t m_end_us;
    int m_thread_num;
  };
}
#endif