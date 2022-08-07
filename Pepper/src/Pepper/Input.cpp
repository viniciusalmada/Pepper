#include "Input.hpp"

bool Pepper::Input::IsKeyPressed(PPKey keyCode)
{
  return instance->IsKeyPressedImpl(keyCode);
}

bool Pepper::Input::IsMouseButtonPressed(PPMouseBt button)
{
  return instance->IsMouseButtonImpl(button);
}

float Pepper::Input::GetMouseX()
{
  auto [x, _] = instance->GetMouseXYImpl();
  return x;
}

float Pepper::Input::GetMouseY()
{
  auto [_, y] = instance->GetMouseXYImpl();
  return y;
}