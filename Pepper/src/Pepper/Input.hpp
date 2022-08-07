#pragma once
// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Core.hpp"
#include "Pepper/KeyCodes.hpp"
#include "Pepper/MouseButtonCodes.hpp"

namespace Pepper
{
  class PEPPER_API Input
  {
  public:
    static bool IsKeyPressed(PPKey keyCode);
    static bool IsMouseButtonPressed(PPMouseBt button);
    static float GetMouseX();
    static float GetMouseY();

  protected:
    virtual bool IsKeyPressedImpl(PPKey keyCode) = 0;
    virtual bool IsMouseButtonImpl(PPMouseBt button) = 0;
    virtual std::pair<float,float> GetMouseXYImpl() = 0;

  private:
    static Input* instance;
  };
}