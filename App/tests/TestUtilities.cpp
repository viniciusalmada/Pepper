#include "Utilities.hpp"

int main()
{
  auto interp = Utils::Interpol(1.0f, 2.0f, 100.0f, 200.0f, 1.5f);

  if (interp == 150.0f)
    return 0;

  return 1;
}