#pragma once

#include "Pepper/Input/Input.hpp"

#include <GLFW/glfw3.h>

namespace Pepper
{
  class WinInput : public Input
  {
  protected:
    bool IsKeyPressedImpl(PPKey keyCode) override;
    bool IsMouseButtonImpl(PPMouseBt button) override;
    std::pair<float, float> GetMouseXYImpl() override;

  private:
    GLFWwindow* GetGLFWWindow();
  };
}