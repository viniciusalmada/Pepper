#pragma once

#include "Pepper/Core/Core.hpp"
#include "Pepper/Input/KeyCodes.hpp"
#include "Pepper/Input/MouseButtonCodes.hpp"

namespace Pepper
{
  class Input
  {
  public:
    Input(const Input&) = delete;
    Input& operator=(const Input&) = delete;

    static bool IsKeyPressed(PPKey keyCode);
    static bool IsMouseButtonPressed(PPMouseBt button);
    static float GetMouseX();
    static float GetMouseY();

  protected:
    Input() = default;

    virtual bool IsKeyPressedImpl(PPKey keyCode) = 0;
    virtual bool IsMouseButtonImpl(PPMouseBt button) = 0;
    virtual std::pair<float, float> GetMouseXYImpl() = 0;

  private:
    static Input* instance;
  };
}