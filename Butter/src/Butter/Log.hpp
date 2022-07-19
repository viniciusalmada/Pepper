#pragma once

#include "Core.hpp"

#include <memory>
// clang-format off
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
// clang-format on

namespace Butter
{
  class BUTTER_API Log
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
#pragma warning(push)
#pragma warning(disable : 4251) // needs to have dll-interface
    static std::shared_ptr<spdlog::logger> core_logger;
    static std::shared_ptr<spdlog::logger> client_logger;
#pragma warning(pop)
  };
}

// Core log macros
#define BT_CORE_TRACE(...) ::Butter::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define BT_CORE_INFO(...) ::Butter::Log::GetCoreLogger()->info(__VA_ARGS__)
#define BT_CORE_WARN(...) ::Butter::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define BT_CORE_ERROR(...) ::Butter::Log::GetCoreLogger()->error(__VA_ARGS__)
#define BT_CORE_FATAL(...) ::Butter::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define BT_TRACE(...) ::Butter::Log::GetClientLogger()->trace(__VA_ARGS__)
#define BT_INFO(...) ::Butter::Log::GetClientLogger()->info(__VA_ARGS__)
#define BT_WARN(...) ::Butter::Log::GetClientLogger()->warn(__VA_ARGS__)
#define BT_ERROR(...) ::Butter::Log::GetClientLogger()->error(__VA_ARGS__)
#define BT_FATAL(...) ::Butter::Log::GetClientLogger()->fatal(__VA_ARGS__)