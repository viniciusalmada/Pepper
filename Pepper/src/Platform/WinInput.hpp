#pragma once

#include "Pepper/Input.hpp"

namespace Pepper
{
  class PEPPER_API WinInput : public Input
  {
  protected:
    bool IsKeyPressedImpl(PPKey keyCode) override;
    bool IsMouseButtonImpl(PPMouseBt button) override;
    float GetMouseXImpl() override;
    float GetMouseYImpl() override;
  };
}