// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Event.hpp"

std::ostream& Pepper::operator<<(std::ostream& os, const Pepper::Event& e) { return os << e.ToString(); }