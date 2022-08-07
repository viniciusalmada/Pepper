#pragma once

struct PPMouseBt
{
  int button;

  friend std::ostream& operator<<(std::ostream& os, PPMouseBt ppBt)
  {
    return os << ppBt.button;
  }

  static PPMouseBt FromGLFWMouse(int glfwMouseBt)
  {
    return PPMouseBt{ glfwMouseBt };
  }

  static int ToGLFWMouse(PPMouseBt ppBt) { return ppBt.button; }
};

// from glfw3.h, but with constexpr
constexpr PPMouseBt PP_MOUSE_BUTTON_1{ 0 };
constexpr PPMouseBt PP_MOUSE_BUTTON_2{ 1 };
constexpr PPMouseBt PP_MOUSE_BUTTON_3{ 2 };
constexpr PPMouseBt PP_MOUSE_BUTTON_4{ 3 };
constexpr PPMouseBt PP_MOUSE_BUTTON_5{ 4 };
constexpr PPMouseBt PP_MOUSE_BUTTON_6{ 5 };
constexpr PPMouseBt PP_MOUSE_BUTTON_7{ 6 };
constexpr PPMouseBt PP_MOUSE_BUTTON_8{ 7 };
constexpr PPMouseBt PP_MOUSE_BUTTON_LAST{ PP_MOUSE_BUTTON_8 };
constexpr PPMouseBt PP_MOUSE_BUTTON_LEFT{ PP_MOUSE_BUTTON_1 };
constexpr PPMouseBt PP_MOUSE_BUTTON_RIGHT{ PP_MOUSE_BUTTON_2 };
constexpr PPMouseBt PP_MOUSE_BUTTON_MIDDLE{ PP_MOUSE_BUTTON_3 };