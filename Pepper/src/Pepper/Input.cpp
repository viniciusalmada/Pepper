#include "Input.hpp"

inline bool Pepper::Input::IsKeyPressed(PPKey keyCode)
{
  return instance->IsKeyPressedImpl(keyCode);
}

inline bool Pepper::Input::IsMouseButtonPressed(PPMouseBt button)
{
  return instance->IsMouseButtonImpl(button);
}

inline float Pepper::Input::GetMouseX() { return instance->GetMouseXImpl(); }

inline float Pepper::Input::GetMouseY() { return instance->GetMouseYImpl(); }