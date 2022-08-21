// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Utils.hpp"

std::string Pepper::Utils::GetNewLine()
{
#ifdef PP_PLATFORM_WINDOWS
  return "\r\n";
#else
  return "\n";
#endif
}