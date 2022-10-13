#if defined(PROFILING)

// clang-format off
#include "PepperPCH.hpp"
// clang-format on

  #include "Pepper/Profiling/Profiler.hpp"

namespace Pepper
{
  class Session
  {
  public:
    static void WriteHeader(std::ofstream& output)
    {
      output << R"({"otherData": {},"traceEvents":[)";
      output.flush();
    }

    static void WriteFooter(std::ofstream& output)
    {
      output << "]}";
      output.flush();
    }

    explicit Session(std::filesystem::path&& file) : m_file(file), m_profiler_timers(), m_stop_session(false)
    {
      PP_CORE_TRACE("New profile session at: {0}", file.string());
    }

    [[nodiscard]] const std::filesystem::path& GetFile() const { return m_file; }

    void Enqueue(Scope<ProfilerData>&& data)
    {
      try
      {
        m_profiler_timers.emplace_back(std::move(data));
      }
      catch (std::exception& e)
      {
        PP_CORE_ERROR("Error inserting data: {0} ({1})", e.what(), data->GetTitle());
      }
    }

    [[nodiscard]] bool IsEmpty() const { return m_profiler_timers.empty(); }

    [[nodiscard]] const ProfilerData& GetTop() { return *m_profiler_timers.front(); }

    void ReleaseTop() { m_profiler_timers.erase(m_profiler_timers.begin()); }

    void LoopSession(std::mutex& lock)
    {
      std::ofstream output{ GetFile() };
      Session::WriteHeader(output);
      while (!m_stop_session)
      {
        if (IsEmpty())
          continue;

        const auto& data = GetTop();

        if (m_timer_count++ > 0)
          output << ",";

        output << data;

        output.flush();

        std::lock_guard<std::mutex> guard{ lock };
        ReleaseTop();
      }
      PP_CORE_TRACE("Wrote footer of session: {0}", GetFile().string());
      Session::WriteFooter(output);
    }

    void StopSession() { m_stop_session = true; }

  private:
    std::filesystem::path m_file;
    std::vector<Scope<ProfilerData>> m_profiler_timers;
    int m_timer_count = 0;
    bool m_stop_session;
  };

  class Profiler::Impl
  {
  public:
    Impl();

    void StartSessionLoop()
    {
      m_writer = std::jthread{ [this] { m_session->LoopSession(m_lock); } };
    }

    std::mutex m_lock;
    std::map<uint64_t, int> m_threads_ids;
    int m_threads_counter;
    bool m_is_active;

    // New approach with specific thread to write the file
    Scope<Session> m_session;
    std::jthread m_writer;
  };

  Profiler::Impl::Impl() : m_lock(), m_threads_ids({}), m_threads_counter(0), m_is_active(false) {}

  Profiler::Profiler() : pimp(CreateScope<Impl>()) {}

  Profiler::~Profiler() = default;

  void Profiler::BeginSession(std::string&& filepath)
  {
    pimp->m_is_active = true;

    uint64_t main_thread_id = std::hash<std::thread::id>{}(std::this_thread::get_id());
    pimp->m_threads_ids[main_thread_id] = pimp->m_threads_counter++;

    pimp->m_session = CreateScope<Session>(std::filesystem::path{ filepath });

    pimp->StartSessionLoop();

    //
    //    // Reset the queue and restart the writer
    //    while (!pimp->m_profiler_timers.empty())
    //    {
    //    }
    //
    //    PP_CORE_INFO("New profiler session path is: {0}", std::filesystem::absolute(filepath));
    //    pimp->m_output.open(filepath);
    //    pimp->WriteHeader();
    //
    //    uint64_t main_thread_id = std::hash<std::thread::id>{}(std::this_thread::get_id());
    //    pimp->m_threads_ids[main_thread_id] = pimp->m_threads_counter++;
  }

  void Profiler::EndSession()
  {
    pimp->m_threads_ids.clear();
    pimp->m_threads_counter = 0;

    pimp->m_session->StopSession();
    if (pimp->m_writer.joinable())
      pimp->m_writer.join();

    pimp->m_is_active = false;
  }

  void Profiler::EnqueueTimer(Scope<ProfilerData>&& data)
  {
    std::lock_guard<std::mutex> lock{ pimp->m_lock };

    if (pimp->m_threads_ids.count(data->GetThread()) == 0)
      pimp->m_threads_ids[data->GetThread()] = pimp->m_threads_counter++;

    pimp->m_session->Enqueue(std::move(data));
  }

  Profiler& Profiler::Get()
  {
    static Profiler self{};
    return self;
  }
  bool Profiler::IsActive() const
  {
    return pimp->m_is_active;
  }

  int Profiler::GetThreadNum(uint64_t threadID) const
  {
    if (pimp->m_threads_ids.count(threadID) != 0)
      return pimp->m_threads_ids[threadID];
    pimp->m_threads_ids[threadID] = pimp->m_threads_counter++;
    return pimp->m_threads_ids[threadID];
  }

}
#endif