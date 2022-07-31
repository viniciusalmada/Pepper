#pragma once

#include "Core.hpp"

namespace Pepper
{
  class PEPPER_API Input
  {
  public:
    static bool IsKeyPressed(int keyCode);
    static bool IsMouseButtonPressed(int button);
    static float GetMouseX();
    static float GetMouseY();

  protected:
    virtual bool IsKeyPressedImpl(int keyCode) = 0;
    virtual bool IsMouseButtonImpl(int button) = 0;
    virtual float GetMouseXImpl() = 0;
    virtual float GetMouseYImpl() = 0;

  private:
    static Input* instance;
  };
}