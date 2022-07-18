#pragma once

#include "Core.hpp"

#include <memory>
#include <spdlog/spdlog.h>

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
    static std::shared_ptr<spdlog::logger> core_logger;
    static std::shared_ptr<spdlog::logger> client_logger;
  };
}