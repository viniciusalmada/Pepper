// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Pepper/Profiling/Profiler.hpp"

namespace Pepper
{
  class Profiler::Impl
  {
  public:
    Impl();

    void WriteHeader();
    void WriteFooter();

    std::ofstream m_output;
    int m_timer_count;
    std::mutex m_lock;
    std::map<uint64_t, int> m_threads_ids;
    int m_threads_counter;
  };

  Profiler::Impl::Impl() : m_output(), m_timer_count(0), m_lock(), m_threads_ids({}), m_threads_counter(0) {}

  void Profiler::Impl::WriteHeader()
  {
    m_output << "{\"otherData\": {},\"traceEvents\":[";
    m_output.flush();
  }

  void Profiler::Impl::WriteFooter()
  {
    m_output << "]}";
    m_output.flush();
  }

  Profiler::Profiler() = default;

  Profiler::~Profiler() = default;

  void Profiler::BeginSession(std::string&& filepath)
  {
    pimp->m_output.open(filepath);
    pimp->WriteHeader();

    uint64_t main_thread_id = std::hash<std::thread::id>{}(std::this_thread::get_id());
    pimp->m_threads_ids[main_thread_id] = pimp->m_threads_counter++;
  }

  void Profiler::EndSession()
  {
    pimp->WriteFooter();
    pimp->m_output.close();
    pimp->m_timer_count = 0;
    pimp->m_threads_ids.clear();
    pimp->m_threads_counter = 0;
  }

  void Profiler::WriteTimer(ProfilerData&& data)
  {
    std::lock_guard<std::mutex> lock{ pimp->m_lock };

    if (pimp->m_threads_ids.count(data.m_thread_hash) == 0)
      pimp->m_threads_ids[data.m_thread_hash] = pimp->m_threads_counter++;

    if (pimp->m_timer_count++ > 0)
      pimp->m_output << ",";

    std::string& title = data.m_title;
    std::replace(title.begin(), title.end(), '"', '\'');

    pimp->m_output << "{";
    pimp->m_output << "\"cat\":\"function\",";
    pimp->m_output << "\"dur\":" << (data.m_end_ns - data.m_start_ns) * 1e-3 << ',';
    pimp->m_output << "\"name\":\"" << title << "\",";
    pimp->m_output << "\"ph\":\"X\",";
    pimp->m_output << "\"pid\":0,";
    pimp->m_output << "\"tid\":" << pimp->m_threads_ids[data.m_thread_hash] << ",";
    pimp->m_output << "\"ts\":" << data.m_start_ns * 1e-3;
    pimp->m_output << "}";

    pimp->m_output.flush();
  }

  Profiler& Profiler::Get()
  {
    static Profiler& self {}
    return self;
  }

}
