//clang-format off
#include "PepperPCH.hpp"
//clang-format on

#include "Pepper/Profiling/ProfilerData.hpp"

namespace Pepper
{
  ProfilerData::ProfilerData(std::string title, uint64_t start, uint64_t end, int threadNum) :
      m_title((title)),
      m_start_us(start),
      m_end_us(end),
      m_thread_num(threadNum)
  {
  }

  uint64_t ProfilerData::GetThread() const
  {
    return m_thread_num;
  }

  const std::string& ProfilerData::GetTitle() const
  {
    return m_title;
  }

  std::ofstream& operator<<(std::ofstream& output, const ProfilerData& data)
  {
    PP_CORE_ASSERT(!data.m_title.empty(), "Name empty!");

    output << "{";
    output << R"("cat":"function",)";
    output << "\"dur\":" << std::max(data.m_end_us, data.m_start_us) - std::min(data.m_end_us, data.m_start_us) << ',';
    output << R"("name":")" << data.m_title << "\",";
    output << R"("ph":"X",)";
    output << "\"pid\":0,";
    output << "\"tid\":" << data.m_thread_num << ",";
    output << "\"ts\":" << data.m_start_us;
    output << "}";

    return output;
  }

  std::weak_ordering ProfilerData::operator<=>(const ProfilerData& other) const
  {
    return m_end_us <=> other.m_end_us;
  }
  //  ProfilerData::ProfilerData(ProfilerData&& other) noexcept :
  //      m_title((other.m_title)),
  //      m_start_us(other.m_start_us),
  //      m_end_us(other.m_end_us),
  //      m_thread_num(other.m_thread_num)
  //  {
  //  }
  //  ProfilerData& ProfilerData::operator=(const ProfilerData& other)
  //  {
  //    m_title = ((other.m_title));
  //    m_start_us = (other.m_start_us);
  //    m_end_us = (other.m_end_us);
  //    m_thread_num = (other.m_thread_num);
  //
  //    return *this;
  //  }
  //
  //  ProfilerData::ProfilerData(const ProfilerData& other) :
  //      m_title((other.m_title)),
  //      m_start_us(other.m_start_us),
  //      m_end_us(other.m_end_us),
  //      m_thread_num(other.m_thread_num)
  //  {
  //  }
  ProfilerData::~ProfilerData()
  {
  }
}