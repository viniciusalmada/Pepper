// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include <spdlog/sinks/stdout_color_sinks.h>

namespace Pepper
{
  std::shared_ptr<spdlog::logger> Log::core_logger;
  std::shared_ptr<spdlog::logger> Log::client_logger;

  void Log::Init()
  {
    /* Pattern:
     * %^: start color range (can be used only once)
     * %T: ISO 8601 time format (HH:MM:SS), equivalent to %H:%M:%S
     * %n: Logger's name
     * %v: The actual text to log
     * %$	end color range (can be used only once)
     */
    spdlog::set_pattern("%^[%T] %n: %v%$");

    core_logger = spdlog::stdout_color_mt("PEPPER");
    core_logger->set_level(spdlog::level::trace);

    client_logger = spdlog::stdout_color_mt("APP");
    client_logger->set_level(spdlog::level::trace);
  }
}