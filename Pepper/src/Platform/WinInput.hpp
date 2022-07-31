#pragma once

#include "Pepper/Input.hpp"

namespace Pepper
{
  class PEPPER_API WinInput : public Input
  {
  protected:
    bool IsKeyPressedImpl(int keyCode) override;
    bool IsMouseButtonImpl(int button) override;
    float GetMouseXImpl() override;
    float GetMouseYImpl() override;
  };
}