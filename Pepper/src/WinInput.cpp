// clang-format off
#include "PepperPCH.hpp"
// clang-format on

#include "Pepper/Platform/Windows/WinInput.hpp"

#include "Pepper/Core/Application.hpp"

#include <GLFW/glfw3.h>

namespace Pepper
{
  Input* Input::instance = new WinInput{};

  class WinInput::Impl
  {
  public:
    GLFWwindow* GetGLFWWindow();
  };

  WinInput::WinInput() : pimp{ new Impl{} } {}

  WinInput::~WinInput() = default;

  bool WinInput::IsKeyPressedImpl(PPKey keyCode)
  {
    GLFWwindow* glfw_window = pimp->GetGLFWWindow();

    int state = glfwGetKey(glfw_window, PPKey::ToGLFWKey(keyCode));

    return state == GLFW_PRESS;
  }

  bool WinInput::IsMouseButtonImpl(PPMouseBt button)
  {
    GLFWwindow* glfw_window = pimp->GetGLFWWindow();

    int state = glfwGetMouseButton(glfw_window, PPMouseBt::ToGLFWMouse(button));

    return state == GLFW_PRESS;
  }

  std::pair<float, float> WinInput::GetMouseXYImpl()
  {
    GLFWwindow* glfw_window = pimp->GetGLFWWindow();
    double x{}, y{};
    glfwGetCursorPos(glfw_window, &x, &y);

    return { static_cast<float>(x), static_cast<float>(y) };
  }

  GLFWwindow* WinInput::Impl::GetGLFWWindow()
  {
    Window& window = Application::Get().GetWindow();
    GLFWwindow* glfw_window = std::any_cast<GLFWwindow*>(window.GetNativeWindow());
    return glfw_window;
  }
}