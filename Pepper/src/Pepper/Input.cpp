#include "Input.hpp"

bool Pepper::Input::IsKeyPressed(PPKey keyCode)
{
  return instance->IsKeyPressedImpl(keyCode);
}

bool Pepper::Input::IsMouseButtonPressed(PPMouseBt button)
{
  return instance->IsMouseButtonImpl(button);
}

float Pepper::Input::GetMouseX() { return instance->GetMouseXImpl(); }

float Pepper::Input::GetMouseY() { return instance->GetMouseYImpl(); }