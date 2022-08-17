#pragma once
// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Pepper/Core/Core.hpp"
#include "Pepper/Input/KeyCodes.hpp"
#include "Pepper/Input/MouseButtonCodes.hpp"

namespace Pepper
{
  class Input
  {
  public:
    static bool IsKeyPressed(PPKey keyCode);
    static bool IsMouseButtonPressed(PPMouseBt button);
    static float GetMouseX();
    static float GetMouseY();

  protected:
    virtual bool IsKeyPressedImpl(PPKey keyCode) = 0;
    virtual bool IsMouseButtonImpl(PPMouseBt button) = 0;
    virtual std::pair<float, float> GetMouseXYImpl() = 0;

  private:
    static Input* instance;
  };
}