#pragma once

#include "Pepper/Input/Input.hpp"

namespace Pepper
{
  class WinInput : public Input
  {
  public:
    WinInput();
    ~WinInput();

  protected:
    bool IsKeyPressedImpl(PPKey keyCode) override;
    bool IsMouseButtonImpl(PPMouseBt button) override;
    std::pair<float, float> GetMouseXYImpl() override;

  private:
    DECLARE_PIMPL
  };
}