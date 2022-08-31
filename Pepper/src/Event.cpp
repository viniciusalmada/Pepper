// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Pepper/Events/Event.hpp"

namespace Pepper
{
  std::ostream& operator<<(std::ostream& os, const Event& e)
  {
    return os << e.ToString();
  }
}