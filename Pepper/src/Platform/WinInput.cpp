// clang-format off
#include "PepperPCH.hpp"
// clang-format on
#include "WinInput.hpp"

#include "Pepper/Application.hpp"

#include <GLFW/glfw3.h>

Pepper::Input* Pepper::Input::instance = new WinInput{};

bool Pepper::WinInput::IsKeyPressedImpl(PPKey keyCode)
{
  Window& window = Application::Get().GetWindow();
  auto glfw_window = static_cast<GLFWwindow*>(window.GetNativeWindow());

  int state = glfwGetKey(glfw_window, PPKey::ToGLFWKey(keyCode));

  return state == GLFW_PRESS;
}

bool Pepper::WinInput::IsMouseButtonImpl(PPMouseBt button)
{
  Window& window = Application::Get().GetWindow();
  auto glfw_window = static_cast<GLFWwindow*>(window.GetNativeWindow());

  int state = glfwGetMouseButton(glfw_window, PPMouseBt::ToGLFWMouse(button));

  return state == GLFW_PRESS;
}

std::pair<float, float> Pepper::WinInput::GetMouseXYImpl()
{
  Window& window = Application::Get().GetWindow();
  auto glfw_window = static_cast<GLFWwindow*>(window.GetNativeWindow());

  double x{}, y{};
  glfwGetCursorPos(glfw_window, &x, &y);

  return { static_cast<float>(x), static_cast<float>(y) };
}