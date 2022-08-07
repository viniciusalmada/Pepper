#pragma once

#include "Pepper/Input.hpp"

namespace Pepper
{
  class PEPPER_API WinInput : public Input
  {
  protected:
    bool IsKeyPressedImpl(PPKey keyCode) override;
    bool IsMouseButtonImpl(PPMouseBt button) override;
    std::pair<float,float> GetMouseXYImpl() override;
  };
}