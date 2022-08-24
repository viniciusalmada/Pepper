// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Utils.hpp"

namespace Pepper::Utils
{
  std::string GetNewLine()
  {
#ifdef PP_PLATFORM_WINDOWS
    return "\r\n";
#else
    return "\n";
#endif
  }
}