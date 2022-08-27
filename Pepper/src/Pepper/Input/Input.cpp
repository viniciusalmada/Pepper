// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Input.hpp"

namespace Pepper
{
  bool Input::IsKeyPressed(PPKey keyCode)
  {
    return instance->IsKeyPressedImpl(keyCode);
  }

  bool Input::IsMouseButtonPressed(PPMouseBt button)
  {
    return instance->IsMouseButtonImpl(button);
  }

  float Input::GetMouseX()
  {
    auto [x, _] = instance->GetMouseXYImpl();
    return x;
  }

  float Input::GetMouseY()
  {
    auto [_, y] = instance->GetMouseXYImpl();
    return y;
  }
}