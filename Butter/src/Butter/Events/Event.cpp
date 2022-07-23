#include "Event.hpp"

std::ostream& Butter::operator<<(std::ostream& os, const Butter::Event& e)
{
  return os << e.ToString();
}