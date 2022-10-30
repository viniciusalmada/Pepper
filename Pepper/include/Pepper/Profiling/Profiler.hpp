#if defined(PROFILING)

  #pragma once

  #include "Pepper/Core/Core.hpp"
  #include "Pepper/Profiling/ProfilerData.hpp"
  #include "Pepper/Profiling/Timer.hpp"

namespace Pepper
{
  class Profiler
  {
  public:
    Profiler();
    ~Profiler();

    void BeginSession(std::string&& filepath);

    void EndSession();

    void WriteTimer(ProfilerData&& data);

    static Profiler& Get();

  private:
    DECLARE_PIMPL
  };
}

  #define PP_PROFILE_BEGIN_SESSION(filepath) ::Pepper::Profiler::Get().BeginSession(filepath);
  #define PP_PROFILE_END_SESSION() ::Pepper::Profiler::Get().EndSession();
  #define PP_PROFILE_SCOPE(title) ::Pepper::Timer COMBINE(timer, __LINE__)(title);
  #if defined(PP_PLATFORM_WINDOWS)
    #define PP_PROFILE_FUNCTION() PP_PROFILE_SCOPE(__FUNCSIG__)
  #elif defined(PP_PLATFORM_LINUX)
    #define PP_PROFILE_FUNCTION() PP_PROFILE_SCOPE(__PRETTY_FUNCTION__)
  #endif

#else

  #define PP_PROFILE_BEGIN_SESSION(filepath) ;
  #define PP_PROFILE_END_SESSION() ;
  #define PP_PROFILE_FUNCTION() ;
  #define PP_PROFILE_SCOPE(name) ;

#endif