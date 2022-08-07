#pragma once

#include "Core.hpp"

// clang-format off
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
// clang-format on

namespace Pepper
{
  class Log
  {
  public:
    static void Init();

    inline static std::shared_ptr<spdlog::logger>& GetCoreLogger()
    {
      return core_logger;
    }
    inline static std::shared_ptr<spdlog::logger>& GetClientLogger()
    {
      return client_logger;
    }

  private:
    static std::shared_ptr<spdlog::logger> core_logger;
    static std::shared_ptr<spdlog::logger> client_logger;
  };
}

// Core log macros
#define PP_CORE_TRACE(...) ::Pepper::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define PP_CORE_INFO(...) ::Pepper::Log::GetCoreLogger()->info(__VA_ARGS__)
#define PP_CORE_WARN(...) ::Pepper::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define PP_CORE_ERROR(...) ::Pepper::Log::GetCoreLogger()->error(__VA_ARGS__)
#define PP_CORE_FATAL(...) ::Pepper::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define PP_TRACE(...) ::Pepper::Log::GetClientLogger()->trace(__VA_ARGS__)
#define PP_INFO(...) ::Pepper::Log::GetClientLogger()->info(__VA_ARGS__)
#define PP_WARN(...) ::Pepper::Log::GetClientLogger()->warn(__VA_ARGS__)
#define PP_ERROR(...) ::Pepper::Log::GetClientLogger()->error(__VA_ARGS__)
#define PP_FATAL(...) ::Pepper::Log::GetClientLogger()->fatal(__VA_ARGS__)